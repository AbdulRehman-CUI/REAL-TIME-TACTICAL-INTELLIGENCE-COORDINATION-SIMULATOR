#ifndef ALGO_CONNECTED_COMPONENTS_H
#define ALGO_CONNECTED_COMPONENTS_H

#include "../DataStructures/Graph.h"

class ConnectedComponents {
public:
    int count(const Graph& graph) const {
        return graph.connectedComponents();
    }
};

#endif
