#ifndef SIM_COMMAND_ENGINE_H
#define SIM_COMMAND_ENGINE_H

#include "../DataStructures/CommandHierarchyTree.h"

class CommandEngine {
    CommandHierarchyTree hierarchy;

public:
    CommandEngine() {
        hierarchy.addUnit(1, 2, "Division Alpha", "Regional Command");
        hierarchy.addUnit(1, 3, "Division Bravo", "Regional Command");
        hierarchy.addUnit(2, 4, "Unit A1", "Rescue Coordination");
        hierarchy.addUnit(2, 5, "Unit A2", "Signal Relay");
        hierarchy.addUnit(3, 6, "Unit B1", "Logistics");
        hierarchy.addUnit(3, 7, "Unit B2", "Monitoring");
    }

    void display() const {
        hierarchy.traverse();
    }
};

#endif
