#ifndef ALGO_PATHFINDING_H
#define ALGO_PATHFINDING_H

#include "../DataStructures/Graph.h"

class Pathfinding {
public:
    DynamicArray<int> safeRoute(const Graph& graph, int sourceId, int targetId, int maxRisk) const {
        return graph.reconstructSafeRoute(sourceId, targetId, maxRisk);
    }
};

#endif
