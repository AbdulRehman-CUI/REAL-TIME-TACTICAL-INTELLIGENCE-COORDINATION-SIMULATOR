#ifndef ALGO_SCC_H
#define ALGO_SCC_H

#include "../DataStructures/Graph.h"

class SCC {
public:
    int estimate(const Graph& graph) const {
        return graph.connectedComponents();
    }
};

#endif
