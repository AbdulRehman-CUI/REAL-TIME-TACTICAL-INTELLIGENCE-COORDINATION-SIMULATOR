from __future__ import annotations

import json
import os
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse


ROOT = Path(__file__).resolve().parent
PROJECT_ROOT = ROOT.parent
ENV_PATH = PROJECT_ROOT / ".env"
COLLECTIONS = ["Nodes", "Edges", "Threats", "Units", "Events", "Operations"]


def load_env() -> None:
    if not ENV_PATH.exists():
        return
    for raw in ENV_PATH.read_text(encoding="utf-8").splitlines():
        line = raw.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, value = line.split("=", 1)
        os.environ.setdefault(key.strip(), value.strip().strip('"').strip("'"))


def mongo_db():
    from pymongo import MongoClient

    uri = os.environ.get("MONGODB_URI", "")
    database = os.environ.get("MONGODB_DATABASE", "Defense_System")
    client = MongoClient(uri, serverSelectionTimeoutMS=8000)
    client.admin.command("ping")
    return client[database]


def json_response(handler: SimpleHTTPRequestHandler, status: int, payload: dict) -> None:
    body = json.dumps(payload, indent=2).encode("utf-8")
    handler.send_response(status)
    handler.send_header("Content-Type", "application/json; charset=utf-8")
    handler.send_header("Content-Length", str(len(body)))
    handler.end_headers()
    handler.wfile.write(body)


class AppHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(ROOT), **kwargs)

    def log_message(self, format: str, *args) -> None:
        return

    def do_GET(self) -> None:
        parsed = urlparse(self.path)
        if parsed.path == "/api/status":
            self.api_status()
            return
        if parsed.path == "/api/snapshot":
            self.api_snapshot()
            return
        if parsed.path == "/":
            self.path = "/demo.html"
        super().do_GET()

    def api_status(self) -> None:
        try:
            db = mongo_db()
            json_response(self, 200, {
                "status": "connected",
                "database": db.name,
                "collections": {name: db[name].count_documents({}) for name in COLLECTIONS},
            })
        except Exception as exc:
            json_response(self, 500, {"status": "error", "message": str(exc)})

    def api_snapshot(self) -> None:
        try:
            db = mongo_db()
            payload = {"status": "connected", "database": db.name}
            for name in COLLECTIONS:
                payload[name] = list(db[name].find({}, {"_id": False}))
            json_response(self, 200, payload)
        except Exception as exc:
            json_response(self, 500, {"status": "error", "message": str(exc)})


if __name__ == "__main__":
    load_env()
    host = "127.0.0.1"
    port = int(os.environ.get("PORT", "4173"))
    server = ThreadingHTTPServer((host, port), AppHandler)
    print(f"Simulator app + MongoDB API available at http://{host}:{port}")
    server.serve_forever()

