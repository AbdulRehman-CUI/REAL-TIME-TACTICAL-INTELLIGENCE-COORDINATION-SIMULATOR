#ifndef DSA_TRIE_H
#define DSA_TRIE_H

#include <cctype>
#include <string>
#include "DynamicArray.h"

class Trie {
    struct Node {
        Node* children[37];
        bool terminal;
        std::string value;

        Node() : terminal(false), value("") {
            for (int i = 0; i < 37; ++i) children[i] = nullptr;
        }
    };

    Node* root;

    // Convert a character into a child index for letters, digits, or space.
    int indexOf(char c) const {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (c >= 'a' && c <= 'z') return c - 'a';
        if (c >= '0' && c <= '9') return 26 + (c - '0');
        if (c == ' ') return 36;
        return -1;
    }

    // Collect autocomplete results from the current subtree until limit is reached.
    void collect(Node* node, DynamicArray<std::string>& out, int limit) const {
        if (!node || out.size() >= limit) return;
        if (node->terminal) out.pushBack(node->value);
        for (int i = 0; i < 37 && out.size() < limit; ++i) {
            collect(node->children[i], out, limit);
        }
    }

    // Destroy a node and all descendants recursively.
    void destroy(Node* node) {
        if (!node) return;
        for (int i = 0; i < 37; ++i) destroy(node->children[i]);
        delete node;
    }

public:
    // Initialize the trie with an empty root node.
    Trie() {
        root = new Node();
    }

    // Recursively free all trie nodes on destruction.
    ~Trie() {
        destroy(root);
    }

    // Insert text into the trie, creating nodes for each supported character.
    void insert(const std::string& text) {
        Node* cur = root;
        for (int i = 0; i < static_cast<int>(text.size()); ++i) {
            int idx = indexOf(text[i]);
            if (idx < 0) continue;
            if (!cur->children[idx]) cur->children[idx] = new Node();
            cur = cur->children[idx];
        }
        cur->terminal = true;
        cur->value = text;
    }

    // Return true if the trie contains the exact text as a terminal word.
    bool contains(const std::string& text) const {
        Node* cur = root;
        for (int i = 0; i < static_cast<int>(text.size()); ++i) {
            int idx = indexOf(text[i]);
            if (idx < 0) continue;
            if (!cur->children[idx]) return false;
            cur = cur->children[idx];
        }
        return cur->terminal;
    }

    // Autocomplete a prefix by collecting up to limit matching strings.
    DynamicArray<std::string> autocomplete(const std::string& prefix, int limit = 8) const {
        DynamicArray<std::string> results;
        Node* cur = root;
        for (int i = 0; i < static_cast<int>(prefix.size()); ++i) {
            int idx = indexOf(prefix[i]);
            if (idx < 0) continue;
            if (!cur->children[idx]) return results;
            cur = cur->children[idx];
        }
        collect(cur, results, limit);
        return results;
    }
};

#endif
