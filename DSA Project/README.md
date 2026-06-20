# Real-Time Tactical Intelligence & Coordination Simulator

Educational simulation platform for demonstrating advanced Data Structures and Algorithms in a tactical coordination setting. This is not a targeting system. It focuses on network analysis, communication reliability, logistics, threat monitoring, event processing, and decision support.

## What Is Included

- C++ backend simulation engine with manually implemented core data structures.
- Offline web dashboard demo with interactive graph, BFS/DFS traversal, threat spread, routing, queue, heap, trie search, hierarchy, union-find, and analytics views.
- React + TypeScript frontend scaffold matching the requested architecture.
- MongoDB collection design and a `MongoManager` adapter placeholder with local fallback semantics.

## No External APIs

The project does not call external APIs or third-party services. Demo data is generated internally.

## Backend DSA Rule

Core algorithmic logic avoids STL containers such as `std::vector`, `std::queue`, `std::stack`, `std::priority_queue`, `std::unordered_map`, `std::map`, and `std::set`. Instead, algorithms use the custom structures in `backend/DataStructures`.

`std::string` and stream utilities are used for labels and output only.

## Quick Start

### Offline Web Demo

Run the app server, then open the URL:

```powershell
& 'C:\Users\J A\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe' frontend\app_server.py
```

`http://127.0.0.1:4173/`

### C++ Backend

From the project root:

```powershell
g++ -std=c++17 backend/main.cpp -Ibackend -o backend/simulator.exe
.\backend\simulator.exe
```

### React/TypeScript Scaffold

The scaffold is in `frontend/src`. It is ready for a normal npm/Vite environment:

```powershell
cd frontend
npm install
npm run dev
```

This local machine did not have npm available through the shell, so the verified runnable UI is the offline demo.

## Project Structure

```text
backend/
  DataStructures/
  Algorithms/
  Simulation/
  Database/
  main.cpp
frontend/
  demo.html
  src/
```

## MongoDB Collections

Schema notes are in `backend/Database/MongoManager.h` and mirrored by the internal simulation model:

- Nodes
- Edges
- Threats
- Units
- Events
- Operations

## Real MongoDB Connection

Create `.env` from `.env.example`, then set:

```text
MONGODB_URI=your MongoDB connection string
MONGODB_DATABASE=tactical_simulator
```

MongoDB database names cannot contain spaces, so use `Defense_System` instead of `Defense System`.

Install the Python bridge dependency if needed:

```powershell
& 'C:\Users\J A\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe' -m pip install pymongo
```

Check the connection:

```powershell
& 'C:\Users\J A\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe' backend\Database\mongo_bridge.py status
```

Seed the simulator collections:

```powershell
& 'C:\Users\J A\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe' backend\Database\mongo_bridge.py seed --drop
```

Run the MongoDB-backed web app server:

```powershell
& 'C:\Users\J A\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe' frontend\app_server.py
```

API checks:

- `http://127.0.0.1:4173/api/status`
- `http://127.0.0.1:4173/api/snapshot`
