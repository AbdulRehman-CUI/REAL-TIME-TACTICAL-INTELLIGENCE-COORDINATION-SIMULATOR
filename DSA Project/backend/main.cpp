#include <iostream>
#include "DataStructures/BinaryTree.h"
#include "DataStructures/CircularQueue.h"
#include "DataStructures/DisjointSet.h"
#include "DataStructures/Graph.h"
#include "DataStructures/Heap.h"
#include "DataStructures/Trie.h"
#include "Simulation/AnalyticsEngine.h"
#include "Simulation/CommandEngine.h"
#include "Simulation/CommunicationEngine.h"
#include "Simulation/ThreatEngine.h"
#include "Database/MongoManager.h"

void printTraversal(const char* label, const DynamicArray<TraversalStep>& steps) {
    std::cout << label << "\n";
    for (int i = 0; i < steps.size(); ++i) {
        std::cout << "  node=" << steps[i].nodeId << " depth=" << steps[i].depth << "\n";
    }
}

void printRoute(const DynamicArray<int>& route) {
    std::cout << "Safe route: ";
    for (int i = 0; i < route.size(); ++i) {
        if (i) std::cout << " -> ";
        std::cout << route[i];
    }
    std::cout << "\n";
}

int main() {
    MongoManager mongo;
    mongo.connect("mongodb://localhost:27017/tactical_sim");
    mongo.describeCollections();

    Graph network(false);
    network.addNode(NodeInfo(101, "Base", "online", 1, 96));
    network.addNode(NodeInfo(102, "Communication Tower", "online", 2, 91));
    network.addNode(NodeInfo(103, "Drone", "online", 3, 75));
    network.addNode(NodeInfo(104, "Rescue Unit", "online", 1, 68));
    network.addNode(NodeInfo(105, "Checkpoint", "online", 4, 62));
    network.addNode(NodeInfo(106, "Supply Center", "online", 2, 88));
    network.addNode(NodeInfo(107, "Enemy Zone", "monitored", 8, 35));
    network.addNode(NodeInfo(108, "Monitoring Station", "online", 2, 80));

    network.addEdge(101, 102, 6, 94, 4, 1);
    network.addEdge(102, 103, 5, 85, 5, 2);
    network.addEdge(103, 105, 8, 70, 9, 5);
    network.addEdge(105, 107, 4, 45, 11, 8);
    network.addEdge(101, 104, 7, 82, 6, 2);
    network.addEdge(104, 106, 3, 76, 3, 1);
    network.addEdge(106, 108, 5, 89, 4, 2);
    network.addEdge(108, 102, 4, 90, 4, 1);

    ThreatEngine threatEngine;
    CommunicationEngine communicationEngine;
    CommandEngine commandEngine;

    printTraversal("\nBFS threat propagation from Enemy Zone 107:", threatEngine.simulatePropagation(network, 107));
    printTraversal("\nDFS communication sweep from Base 101:", network.dfs(101));
    printRoute(network.reconstructSafeRoute(101, 108, 3));

    DynamicArray<int> critical = communicationEngine.criticalTowers(network);
    std::cout << "Articulation points:";
    for (int i = 0; i < critical.size(); ++i) std::cout << " " << critical[i];
    std::cout << "\nConnected regions: " << communicationEngine.connectedRegions(network) << "\n";

    MaxHeap priorityCenter;
    priorityCenter.push(PriorityEvent(1, 95, "Tower outage", "North Relay"));
    priorityCenter.push(PriorityEvent(2, 88, "Supply shortage", "Echo Depot"));
    priorityCenter.push(PriorityEvent(3, 99, "Threat surge", "Zone 7"));
    std::cout << "Top priority: " << priorityCenter.peek().label << " at " << priorityCenter.peek().location << "\n";

    Trie intelligenceSearch;
    intelligenceSearch.insert("Alpha Base");
    intelligenceSearch.insert("Alpha Squadron");
    intelligenceSearch.insert("Alpha Zone");
    intelligenceSearch.insert("Bravo Relay");
    DynamicArray<std::string> suggestions = intelligenceSearch.autocomplete("alp");
    std::cout << "Trie autocomplete for 'alp':";
    for (int i = 0; i < suggestions.size(); ++i) std::cout << " " << suggestions[i];
    std::cout << "\n";

    CircularQueue<std::string> eventQueue(5);
    eventQueue.enqueue("Radar detection");
    eventQueue.enqueue("Supply request");
    eventQueue.enqueue("Emergency call");
    std::string processed;
    eventQueue.dequeue(processed);
    std::cout << "Processed FIFO event: " << processed << "\n";

    DisjointSet regions(4);
    regions.unite(0, 1);
    regions.unite(2, 3);
    std::cout << "Union-Find region 0 connected to 1: " << (regions.connected(0, 1) ? "yes" : "no") << "\n";

    int metrics[6] = {12, 18, 9, 30, 16, 22};
    AnalyticsEngine analytics(metrics, 6);
    std::cout << "Segment tree threat density regions 1..4: " << analytics.queryThreatDensity(1, 4) << "\n";
    analytics.updateRegion(2, 25);
    std::cout << "After point update regions 1..4: " << analytics.queryThreatDensity(1, 4) << "\n";

    BinaryTree regionTree;
    regionTree.insert(4, 30);
    regionTree.insert(2, 18);
    regionTree.insert(7, 42);
    std::cout << "Binary tree in-order analytics index:\n";
    regionTree.inorder();

    std::cout << "\nCommand hierarchy:\n";
    commandEngine.display();

    std::cout << "\nNetwork nodes=" << network.nodeCount() << " edges=" << network.edgeCount() << "\n";
    return 0;
}
