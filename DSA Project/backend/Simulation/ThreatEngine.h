#ifndef SIM_THREAT_ENGINE_H
#define SIM_THREAT_ENGINE_H

#include "../DataStructures/Graph.h"

class ThreatEngine {
public:
    DynamicArray<TraversalStep> simulatePropagation(const Graph& graph, int originNodeId) const {
        return graph.bfs(originNodeId);
    }

    DynamicArray<TraversalStep> simulateMultiSourceSpread(const Graph& graph, const DynamicArray<int>& origins) const {
        return graph.multiSourceBfs(origins);
    }
};

#endif
