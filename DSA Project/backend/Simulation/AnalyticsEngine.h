#ifndef SIM_ANALYTICS_ENGINE_H
#define SIM_ANALYTICS_ENGINE_H

#include "../DataStructures/SegmentTree.h"

class AnalyticsEngine {
    SegmentTree* threatDensity;

public:
    AnalyticsEngine(const int* values, int count) {
        threatDensity = new SegmentTree(values, count);
    }

    ~AnalyticsEngine() {
        delete threatDensity;
    }

    int queryThreatDensity(int leftRegion, int rightRegion) const {
        return threatDensity->rangeQuery(leftRegion, rightRegion);
    }

    void updateRegion(int region, int value) {
        threatDensity->pointUpdate(region, value);
    }
};

#endif
