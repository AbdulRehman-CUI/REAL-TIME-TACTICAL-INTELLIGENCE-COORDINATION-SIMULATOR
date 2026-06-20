from __future__ import annotations

import argparse
import json
import os
import sys
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
ENV_PATH = PROJECT_ROOT / ".env"


def load_env() -> None:
    if not ENV_PATH.exists():
        return
    for raw_line in ENV_PATH.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, value = line.split("=", 1)
        os.environ.setdefault(key.strip(), value.strip().strip('"').strip("'"))


def get_client():
    try:
        from pymongo import MongoClient
        from pymongo.errors import PyMongoError
    except ImportError:
        print("pymongo is not installed. Run: python -m pip install pymongo", file=sys.stderr)
        raise SystemExit(2)

    uri = os.environ.get("MONGODB_URI", "").strip()
    if not uri:
        print("MONGODB_URI is missing. Add it to .env or your environment.", file=sys.stderr)
        raise SystemExit(2)

    try:
        client = MongoClient(uri, serverSelectionTimeoutMS=8000)
        client.admin.command("ping")
        return client
    except PyMongoError as exc:
        print(f"MongoDB connection failed: {exc}", file=sys.stderr)
        raise SystemExit(1)


def database_name() -> str:
    return os.environ.get("MONGODB_DATABASE", "tactical_simulator").strip() or "tactical_simulator"


def seed_data() -> dict[str, list[dict]]:
    return {
        "Nodes": [
            {"nodeId": 101, "nodeType": "Base", "threatLevel": 1, "status": "online", "metadata": {"name": "Alpha Base", "connectivityScore": 96}},
            {"nodeId": 102, "nodeType": "Communication Tower", "threatLevel": 2, "status": "online", "metadata": {"name": "North Relay", "connectivityScore": 91}},
            {"nodeId": 103, "nodeType": "Drone", "threatLevel": 3, "status": "online", "metadata": {"name": "Drone D-14", "connectivityScore": 75}},
            {"nodeId": 104, "nodeType": "Rescue Unit", "threatLevel": 1, "status": "online", "metadata": {"name": "Rescue Unit", "connectivityScore": 68}},
            {"nodeId": 105, "nodeType": "Checkpoint", "threatLevel": 4, "status": "online", "metadata": {"name": "Delta Checkpoint", "connectivityScore": 62}},
            {"nodeId": 106, "nodeType": "Supply Center", "threatLevel": 2, "status": "online", "metadata": {"name": "Echo Supply", "connectivityScore": 88}},
            {"nodeId": 107, "nodeType": "Enemy Zone", "threatLevel": 8, "status": "monitored", "metadata": {"name": "Zone Seven", "connectivityScore": 35}},
            {"nodeId": 108, "nodeType": "Monitoring Station", "threatLevel": 2, "status": "online", "metadata": {"name": "Signal Watch", "connectivityScore": 80}},
        ],
        "Edges": [
            {"edgeId": "e1", "source": 101, "destination": 102, "distance": 6, "riskLevel": 1, "signalStrength": 94},
            {"edgeId": "e2", "source": 102, "destination": 103, "distance": 5, "riskLevel": 2, "signalStrength": 85},
            {"edgeId": "e3", "source": 103, "destination": 105, "distance": 8, "riskLevel": 5, "signalStrength": 70},
            {"edgeId": "e4", "source": 105, "destination": 107, "distance": 4, "riskLevel": 8, "signalStrength": 45},
            {"edgeId": "e5", "source": 101, "destination": 104, "distance": 7, "riskLevel": 2, "signalStrength": 82},
            {"edgeId": "e6", "source": 104, "destination": 106, "distance": 3, "riskLevel": 1, "signalStrength": 76},
            {"edgeId": "e7", "source": 106, "destination": 108, "distance": 5, "riskLevel": 2, "signalStrength": 89},
            {"edgeId": "e8", "source": 108, "destination": 102, "distance": 4, "riskLevel": 1, "signalStrength": 90},
        ],
        "Threats": [
            {"threatId": 1, "priority": 99, "location": "Zone Seven", "status": "active"},
            {"threatId": 2, "priority": 95, "location": "North Relay", "status": "investigating"},
            {"threatId": 3, "priority": 88, "location": "Echo Supply", "status": "queued"},
        ],
        "Units": [
            {"unitId": "A1", "type": "Rescue Coordination", "status": "ready", "location": "Alpha Base"},
            {"unitId": "A2", "type": "Signal Relay", "status": "ready", "location": "North Relay"},
            {"unitId": "B1", "type": "Logistics", "status": "moving", "location": "Echo Supply"},
            {"unitId": "B2", "type": "Monitoring", "status": "active", "location": "Signal Watch"},
        ],
        "Events": [
            {"eventId": 1, "eventType": "Radar Detection", "timestamp": "2026-05-31T00:00:00Z", "status": "pending"},
            {"eventId": 2, "eventType": "Supply Request", "timestamp": "2026-05-31T00:03:00Z", "status": "pending"},
            {"eventId": 3, "eventType": "Emergency Call", "timestamp": "2026-05-31T00:06:00Z", "status": "pending"},
        ],
        "Operations": [
            {"operationId": 1, "operationName": "Operation Meridian", "description": "Educational graph traversal and connectivity scenario"},
            {"operationId": 2, "operationName": "Operation Signal Net", "description": "Communication reliability simulation"},
        ],
    }


def seed_database(drop: bool) -> None:
    load_env()
    client = get_client()
    db = client[database_name()]
    data = seed_data()
    for collection, documents in data.items():
        if drop:
            db[collection].delete_many({})
        if documents:
            db[collection].insert_many(documents)
    print(json.dumps({"status": "seeded", "database": db.name, "collections": list(data.keys())}, indent=2))


def status() -> None:
    load_env()
    client = get_client()
    db = client[database_name()]
    result = {"status": "connected", "database": db.name, "collections": {}}
    for name in ["Nodes", "Edges", "Threats", "Units", "Events", "Operations"]:
        result["collections"][name] = db[name].count_documents({})
    print(json.dumps(result, indent=2))


def export_database(path: Path) -> None:
    load_env()
    client = get_client()
    db = client[database_name()]
    output = {}
    for name in ["Nodes", "Edges", "Threats", "Units", "Events", "Operations"]:
        rows = []
        for doc in db[name].find({}, {"_id": False}):
            rows.append(doc)
        output[name] = rows
    path.write_text(json.dumps(output, indent=2), encoding="utf-8")
    print(json.dumps({"status": "exported", "path": str(path)}, indent=2))


def main() -> None:
    parser = argparse.ArgumentParser(description="MongoDB bridge for the tactical DSA simulator.")
    parser.add_argument("command", choices=["status", "seed", "export"])
    parser.add_argument("--drop", action="store_true", help="Clear target collections before seeding.")
    parser.add_argument("--out", default=str(PROJECT_ROOT / "backend" / "Database" / "mongo_export.json"))
    args = parser.parse_args()

    if args.command == "status":
        status()
    elif args.command == "seed":
        seed_database(drop=args.drop)
    elif args.command == "export":
        export_database(Path(args.out))


if __name__ == "__main__":
    main()

