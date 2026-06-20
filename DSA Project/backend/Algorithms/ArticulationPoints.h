#ifndef ALGO_ARTICULATION_POINTS_H
#define ALGO_ARTICULATION_POINTS_H

#include "../DataStructures/Graph.h"

class ArticulationPoints {
public:
    DynamicArray<int> run(const Graph& graph) const {
        return graph.articulationPoints();
    }
};

#endif
