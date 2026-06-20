import http from "node:http";
import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";

const root = path.dirname(fileURLToPath(import.meta.url));
const rootResolved = path.resolve(root);
const port = Number(process.env.PORT || 4173);
const types = {
  ".html": "text/html; charset=utf-8",
  ".css": "text/css; charset=utf-8",
  ".js": "text/javascript; charset=utf-8",
  ".json": "application/json; charset=utf-8"
};

const server = http.createServer((request, response) => {
  const urlPath = decodeURIComponent(new URL(request.url || "/", `http://localhost:${port}`).pathname);
  const relativePath = urlPath.replace(/^\/+/, "") || "demo.html";
  const safePath = path.normalize(relativePath).replace(/^(\.\.[/\\])+/, "");
  const filePath = path.resolve(root, safePath);
  if (!filePath.toLowerCase().startsWith(rootResolved.toLowerCase())) {
    response.writeHead(403);
    response.end("Forbidden");
    return;
  }
  fs.readFile(filePath, (error, data) => {
    if (error) {
      response.writeHead(404);
      response.end("Not found");
      return;
    }
    response.writeHead(200, { "Content-Type": types[path.extname(filePath)] || "application/octet-stream" });
    response.end(data);
  });
});

server.listen(port, "127.0.0.1", () => {
  console.log(`Simulator demo available at http://127.0.0.1:${port}`);
});
