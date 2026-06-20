#ifndef ALGO_DFS_H
#define ALGO_DFS_H

#include "../DataStructures/Graph.h"

class DFSAlgorithm {
public:
    DynamicArray<TraversalStep> run(const Graph& graph, int startId) const {
        return graph.dfs(startId);
    }
};

#endif
