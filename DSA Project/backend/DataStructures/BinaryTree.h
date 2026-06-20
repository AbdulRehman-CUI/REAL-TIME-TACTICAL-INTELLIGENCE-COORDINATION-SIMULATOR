#ifndef DSA_BINARY_TREE_H
#define DSA_BINARY_TREE_H

#include <iostream>

class BinaryTree {
    // Node represents a single tree element with a key, metric, and left/right children.
    struct Node {
        int key;
        int metric;
        Node* left;
        Node* right;
        Node(int k, int m) : key(k), metric(m), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Insert a node into the subtree rooted at node, preserving binary search order by key.
    // If the key already exists in the subtree, update its metric.
    Node* insert(Node* node, int key, int metric) {
        if (!node) return new Node(key, metric);
        if (key < node->key) node->left = insert(node->left, key, metric);
        else if (key > node->key) node->right = insert(node->right, key, metric);
        else node->metric = metric;
        return node;
    }

    // Traverse the subtree in sorted order and print each visited node's key and metric.
    void inorder(Node* node) const {
        if (!node) return;
        inorder(node->left);
        std::cout << "Region " << node->key << " metric=" << node->metric << "\n";
        inorder(node->right);
    }

    // Recursively delete all nodes in the subtree to release allocated memory.
    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    // Create an empty binary tree with no root node.
    BinaryTree() : root(nullptr) {}

    // Clean up the tree and free all allocated node memory.
    ~BinaryTree() {
        destroy(root);
    }

    // Insert or update a node by key in the tree, maintaining binary search order.
    void insert(int key, int metric) {
        root = insert(root, key, metric);
    }

    // Perform an in-order traversal from the tree root and print node values.
    void inorder() const {
        inorder(root);
    }
};

#endif
