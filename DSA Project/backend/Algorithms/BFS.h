#ifndef ALGO_BFS_H
#define ALGO_BFS_H

#include "../DataStructures/Graph.h"

class BFSAlgorithm {
public:
    DynamicArray<TraversalStep> run(const Graph& graph, int startId) const {
        return graph.bfs(startId);
    }
};

#endif
