#ifndef SIM_COMMUNICATION_ENGINE_H
#define SIM_COMMUNICATION_ENGINE_H

#include "../DataStructures/Graph.h"

class CommunicationEngine {
public:
    DynamicArray<int> criticalTowers(const Graph& graph) const {
        return graph.articulationPoints();
    }

    int connectedRegions(const Graph& graph) const {
        return graph.connectedComponents();
    }
};

#endif
