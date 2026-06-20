#ifndef DSA_COMMAND_HIERARCHY_TREE_H
#define DSA_COMMAND_HIERARCHY_TREE_H

#include <iostream>
#include <string>
#include "DynamicArray.h"

class CommandHierarchyTree {
    // UnitNode stores a hierarchical command unit with parent/child links.
    struct UnitNode {
        int id;
        std::string name;
        std::string role;
        UnitNode* parent;
        DynamicArray<UnitNode*> children;

        UnitNode(int i, const std::string& n, const std::string& r)
            : id(i), name(n), role(r), parent(nullptr), children() {}
    };

    UnitNode* root;

    // Recursively search for a unit node with the given id starting from node.
    UnitNode* find(UnitNode* node, int id) const {
        if (!node) return nullptr;
        if (node->id == id) return node;
        for (int i = 0; i < node->children.size(); ++i) {
            UnitNode* found = find(node->children[i], id);
            if (found) return found;
        }
        return nullptr;
    }

    // Free the node and all descendant nodes recursively.
    void destroy(UnitNode* node) {
        if (!node) return;
        for (int i = 0; i < node->children.size(); ++i) destroy(node->children[i]);
        delete node;
    }

    // Print the hierarchy structure from this node with indentation according to depth.
    void print(UnitNode* node, int depth) const {
        if (!node) return;
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << "- " << node->name << " (" << node->role << ")\n";
        for (int i = 0; i < node->children.size(); ++i) print(node->children[i], depth + 1);
    }

public:
    // Initialize the command hierarchy tree with a root commander node.
    CommandHierarchyTree() {
        root = new UnitNode(1, "Commander", "Strategic Command");
    }

    // Clean up the hierarchy tree and delete all nodes.
    ~CommandHierarchyTree() {
        destroy(root);
    }

    // Add a new unit under the specified parent node, ensuring unique ids.
    bool addUnit(int parentId, int id, const std::string& name, const std::string& role) {
        UnitNode* parent = find(root, parentId);
        if (!parent || find(root, id)) return false;
        UnitNode* child = new UnitNode(id, name, role);
        child->parent = parent;
        parent->children.pushBack(child);
        return true;
    }

    // Print the entire command hierarchy starting from the root node.
    void traverse() const {
        print(root, 0);
    }
};

#endif
