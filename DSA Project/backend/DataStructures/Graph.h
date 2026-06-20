#ifndef DSA_GRAPH_H
#define DSA_GRAPH_H

#include <iostream>
#include <string>
#include "DynamicArray.h"
#include "HashTable.h"
#include "Queue.h"
#include "Stack.h"

// NodeInfo holds metadata for a graph node, including connectivity and threat state.
struct NodeInfo {
    int id;
    std::string type;
    std::string status;
    int threatLevel;
    int connectivityScore;

    NodeInfo(int i = 0, const std::string& t = "", const std::string& s = "online", int th = 0, int cs = 0)
        : id(i), type(t), status(s), threatLevel(th), connectivityScore(cs) {}
};

// EdgeInfo represents an edge with metrics used for routing and risk evaluation.
struct EdgeInfo {
    int destination;
    int distance;
    int signalStrength;
    int travelCost;
    int riskLevel;
    bool active;

    EdgeInfo(int d = 0, int dist = 0, int signal = 0, int cost = 0, int risk = 0)
        : destination(d), distance(dist), signalStrength(signal), travelCost(cost), riskLevel(risk), active(true) {}
};

// TraversalStep captures the discovery order and search depth for graph traversals.
struct TraversalStep {
    int nodeId;
    int depth;
    TraversalStep(int n = 0, int d = 0) : nodeId(n), depth(d) {}
};

// Graph stores nodes and adjacency lists to support traversal and analysis.
class Graph {
    DynamicArray<NodeInfo> nodes;
    DynamicArray<DynamicArray<EdgeInfo> > adjacency;
    HashTable<int> indexById;
    bool directed;

    // Look up the internal index for a node id, or return -1 if missing.
    int getIndex(int nodeId) const {
        int index = -1;
        if (indexById.get(nodeId, index)) return index;
        return -1;
    }

    // DFS helper for articulation point detection on the graph.
    void articulationDfs(int u, bool* visited, int* disc, int* low, int* parent, bool* ap, int& timer) const {
        visited[u] = true;
        disc[u] = low[u] = ++timer;
        int children = 0;
        for (int i = 0; i < adjacency[u].size(); ++i) {
            if (!adjacency[u][i].active) continue;
            int v = getIndex(adjacency[u][i].destination);
            if (v < 0) continue;
            if (!visited[v]) {
                ++children;
                parent[v] = u;
                articulationDfs(v, visited, disc, low, parent, ap, timer);
                low[u] = low[u] < low[v] ? low[u] : low[v];
                if (parent[u] == -1 && children > 1) ap[u] = true;
                if (parent[u] != -1 && low[v] >= disc[u]) ap[u] = true;
            } else if (v != parent[u]) {
                low[u] = low[u] < disc[v] ? low[u] : disc[v];
            }
        }
    }

    // Recursively perform depth-first search and record traversal order from a given node.
    void dfsRecursive(int index, bool* visited, DynamicArray<TraversalStep>& order, int depth) const {
        visited[index] = true;
        order.pushBack(TraversalStep(nodes[index].id, depth));
        for (int i = 0; i < adjacency[index].size(); ++i) {
            if (!adjacency[index][i].active) continue;
            int next = getIndex(adjacency[index][i].destination);
            if (next >= 0 && !visited[next]) dfsRecursive(next, visited, order, depth + 1);
        }
    }

public:
    // Create an empty graph, either directed or undirected.
    Graph(bool isDirected = false) : nodes(), adjacency(), indexById(), directed(isDirected) {}

    // Add a new node to the graph if the id is not already present.
    bool addNode(const NodeInfo& node) {
        if (getIndex(node.id) >= 0) return false;
        nodes.pushBack(node);
        adjacency.pushBack(DynamicArray<EdgeInfo>());
        indexById.put(node.id, nodes.size() - 1);
        return true;
    }

    // Update metadata for an existing node within the graph.
    bool updateNode(const NodeInfo& node) {
        int index = getIndex(node.id);
        if (index < 0) return false;
        nodes[index] = node;
        return true;
    }

    // Add an edge between source and destination with associated metrics.
    bool addEdge(int sourceId, int destId, int distance, int signal, int cost, int risk) {
        int s = getIndex(sourceId);
        int d = getIndex(destId);
        if (s < 0 || d < 0) return false;
        adjacency[s].pushBack(EdgeInfo(destId, distance, signal, cost, risk));
        if (!directed) adjacency[d].pushBack(EdgeInfo(sourceId, distance, signal, cost, risk));
        return true;
    }

    // Remove the edge from source to destination, and if undirected remove the reverse link.
    bool removeEdge(int sourceId, int destId) {
        int s = getIndex(sourceId);
        if (s < 0) return false;
        bool removed = false;
        for (int i = 0; i < adjacency[s].size(); ++i) {
            if (adjacency[s][i].destination == destId) {
                adjacency[s].removeAt(i);
                removed = true;
                break;
            }
        }
        if (!directed) {
            int d = getIndex(destId);
            if (d >= 0) {
                for (int i = 0; i < adjacency[d].size(); ++i) {
                    if (adjacency[d][i].destination == sourceId) {
                        adjacency[d].removeAt(i);
                        break;
                    }
                }
            }
        }
        return removed;
    }

    // Update the travel cost and risk for an existing edge in the graph.
    bool updateEdgeWeight(int sourceId, int destId, int cost, int risk) {
        int s = getIndex(sourceId);
        if (s < 0) return false;
        bool updated = false;
        for (int i = 0; i < adjacency[s].size(); ++i) {
            if (adjacency[s][i].destination == destId) {
                adjacency[s][i].travelCost = cost;
                adjacency[s][i].riskLevel = risk;
                updated = true;
            }
        }
        if (!directed) {
            int d = getIndex(destId);
            if (d >= 0) {
                for (int i = 0; i < adjacency[d].size(); ++i) {
                    if (adjacency[d][i].destination == sourceId) {
                        adjacency[d][i].travelCost = cost;
                        adjacency[d][i].riskLevel = risk;
                    }
                }
            }
        }
        return updated;
    }

    // Breadth-first search from the start node, returning a traversal order and depth per node.
    DynamicArray<TraversalStep> bfs(int startId) const {
        DynamicArray<TraversalStep> order;
        int start = getIndex(startId);
        if (start < 0) return order;
        bool* visited = new bool[nodes.size()];
        int* depth = new int[nodes.size()];
        for (int i = 0; i < nodes.size(); ++i) {
            visited[i] = false;
            depth[i] = 0;
        }
        Queue<int> q;
        visited[start] = true;
        q.enqueue(start);
        while (!q.empty()) {
            int current = q.dequeue();
            order.pushBack(TraversalStep(nodes[current].id, depth[current]));
            for (int i = 0; i < adjacency[current].size(); ++i) {
                if (!adjacency[current][i].active) continue;
                int next = getIndex(adjacency[current][i].destination);
                if (next >= 0 && !visited[next] && nodes[next].status != "disabled") {
                    visited[next] = true;
                    depth[next] = depth[current] + 1;
                    q.enqueue(next);
                }
            }
        }
        delete[] visited;
        delete[] depth;
        return order;
    }

    // Perform BFS from multiple source nodes and collect a combined traversal order.
    DynamicArray<TraversalStep> multiSourceBfs(const DynamicArray<int>& sources) const {
        DynamicArray<TraversalStep> order;
        bool* visited = new bool[nodes.size()];
        int* depth = new int[nodes.size()];
        for (int i = 0; i < nodes.size(); ++i) {
            visited[i] = false;
            depth[i] = 0;
        }
        Queue<int> q;
        for (int i = 0; i < sources.size(); ++i) {
            int index = getIndex(sources[i]);
            if (index >= 0 && !visited[index]) {
                visited[index] = true;
                q.enqueue(index);
            }
        }
        while (!q.empty()) {
            int current = q.dequeue();
            order.pushBack(TraversalStep(nodes[current].id, depth[current]));
            for (int i = 0; i < adjacency[current].size(); ++i) {
                int next = getIndex(adjacency[current][i].destination);
                if (next >= 0 && !visited[next]) {
                    visited[next] = true;
                    depth[next] = depth[current] + 1;
                    q.enqueue(next);
                }
            }
        }
        delete[] visited;
        delete[] depth;
        return order;
    }

    // Depth-first search starting from the specified node id.
    DynamicArray<TraversalStep> dfs(int startId) const {
        DynamicArray<TraversalStep> order;
        int start = getIndex(startId);
        if (start < 0) return order;
        bool* visited = new bool[nodes.size()];
        for (int i = 0; i < nodes.size(); ++i) visited[i] = false;
        dfsRecursive(start, visited, order, 0);
        delete[] visited;
        return order;
    }

    // Count connected components in the graph using breadth-first search.
    int connectedComponents() const {
        bool* visited = new bool[nodes.size()];
        for (int i = 0; i < nodes.size(); ++i) visited[i] = false;
        int components = 0;
        for (int i = 0; i < nodes.size(); ++i) {
            if (!visited[i]) {
                ++components;
                Queue<int> q;
                visited[i] = true;
                q.enqueue(i);
                while (!q.empty()) {
                    int current = q.dequeue();
                    for (int e = 0; e < adjacency[current].size(); ++e) {
                        int next = getIndex(adjacency[current][e].destination);
                        if (next >= 0 && !visited[next]) {
                            visited[next] = true;
                            q.enqueue(next);
                        }
                    }
                }
            }
        }
        delete[] visited;
        return components;
    }

    // Find articulation points whose removal would increase the number of connected components.
    DynamicArray<int> articulationPoints() const {
        DynamicArray<int> points;
        int n = nodes.size();
        bool* visited = new bool[n];
        bool* ap = new bool[n];
        int* disc = new int[n];
        int* low = new int[n];
        int* parent = new int[n];
        for (int i = 0; i < n; ++i) {
            visited[i] = false;
            ap[i] = false;
            disc[i] = 0;
            low[i] = 0;
            parent[i] = -1;
        }
        int timer = 0;
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) articulationDfs(i, visited, disc, low, parent, ap, timer);
        }
        for (int i = 0; i < n; ++i) if (ap[i]) points.pushBack(nodes[i].id);
        delete[] visited;
        delete[] ap;
        delete[] disc;
        delete[] low;
        delete[] parent;
        return points;
    }

    // Reconstruct a safe route from source to target using BFS constrained by maximum risk.
    DynamicArray<int> reconstructSafeRoute(int sourceId, int targetId, int maxRisk) const {
        DynamicArray<int> path;
        int source = getIndex(sourceId);
        int target = getIndex(targetId);
        if (source < 0 || target < 0) return path;
        bool* visited = new bool[nodes.size()];
        int* parent = new int[nodes.size()];
        for (int i = 0; i < nodes.size(); ++i) {
            visited[i] = false;
            parent[i] = -1;
        }
        Queue<int> q;
        visited[source] = true;
        q.enqueue(source);
        while (!q.empty()) {
            int current = q.dequeue();
            if (current == target) break;
            for (int i = 0; i < adjacency[current].size(); ++i) {
                EdgeInfo edge = adjacency[current][i];
                int next = getIndex(edge.destination);
                if (next >= 0 && !visited[next] && edge.riskLevel <= maxRisk && nodes[next].threatLevel <= maxRisk && nodes[next].status != "blocked") {
                    visited[next] = true;
                    parent[next] = current;
                    q.enqueue(next);
                }
            }
        }
        if (visited[target]) {
            Stack<int> stack;
            int cur = target;
            while (cur != -1) {
                stack.push(nodes[cur].id);
                cur = parent[cur];
            }
            while (!stack.empty()) path.pushBack(stack.pop());
        }
        delete[] visited;
        delete[] parent;
        return path;
    }

    // Return the number of nodes stored in the graph.
    int nodeCount() const { return nodes.size(); }

    // Return the number of edges in the graph, adjusted for directed graphs.
    int edgeCount() const {
        int total = 0;
        for (int i = 0; i < adjacency.size(); ++i) total += adjacency[i].size();
        return directed ? total : total / 2;
    }
};

#endif
