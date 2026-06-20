#ifndef DSA_DISJOINT_SET_H
#define DSA_DISJOINT_SET_H

class DisjointSet {
    int* parent;
    int* rank;
    int count;

public:
    // DisjointSet maintains union-find state for a collection of elements.
    DisjointSet(int n = 0) : count(n) {
        parent = new int[count];
        rank = new int[count];
        for (int i = 0; i < count; ++i) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    // Free allocated arrays storing the disjoint set structure.
    ~DisjointSet() {
        delete[] parent;
        delete[] rank;
    }

    // Find the representative parent of element x using path compression.
    // This flattens the structure for future faster lookups.
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    // Union the sets containing a and b by rank to keep the tree shallow.
    void unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);
        if (rootA == rootB) return;
        if (rank[rootA] < rank[rootB]) parent[rootA] = rootB;
        else if (rank[rootA] > rank[rootB]) parent[rootB] = rootA;
        else {
            parent[rootB] = rootA;
            ++rank[rootA];
        }
    }

    // Determine whether a and b belong to the same connected component.
    bool connected(int a, int b) {
        return find(a) == find(b);
    }
};

#endif
