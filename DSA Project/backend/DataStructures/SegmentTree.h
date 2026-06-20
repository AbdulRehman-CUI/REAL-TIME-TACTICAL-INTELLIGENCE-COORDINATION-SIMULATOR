#ifndef DSA_SEGMENT_TREE_H
#define DSA_SEGMENT_TREE_H

class SegmentTree {
    int* tree;
    int size;

    // Build the tree recursively from an input array of values.
    // Each node stores the sum of its segment.
    void build(int node, int left, int right, const int* values) {
        if (left == right) {
            tree[node] = values[left];
            return;
        }
        int mid = (left + right) / 2;
        build(node * 2, left, mid, values);
        build(node * 2 + 1, mid + 1, right, values);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }

    // Query the sum for a range inside the current node's segment.
    int query(int node, int left, int right, int ql, int qr) const {
        if (qr < left || right < ql) return 0;
        if (ql <= left && right <= qr) return tree[node];
        int mid = (left + right) / 2;
        return query(node * 2, left, mid, ql, qr) + query(node * 2 + 1, mid + 1, right, ql, qr);
    }

    // Update a single position within the current node's segment.
    void update(int node, int left, int right, int index, int value) {
        if (left == right) {
            tree[node] = value;
            return;
        }
        int mid = (left + right) / 2;
        if (index <= mid) update(node * 2, left, mid, index, value);
        else update(node * 2 + 1, mid + 1, right, index, value);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }

public:
    // Construct a segment tree from an array of integer values.
    SegmentTree(const int* values, int n) : size(n) {
        tree = new int[size * 4 + 4];
        build(1, 0, size - 1, values);
    }

    // Clean up the allocated tree buffer.
    ~SegmentTree() {
        delete[] tree;
    }

    // Query the sum over a range [left, right] clamped to valid bounds.
    int rangeQuery(int left, int right) const {
        if (left < 0) left = 0;
        if (right >= size) right = size - 1;
        if (left > right) return 0;
        return query(1, 0, size - 1, left, right);
    }

    // Update a single element in the original array and propagate changes.
    void pointUpdate(int index, int value) {
        if (index < 0 || index >= size) return;
        update(1, 0, size - 1, index, value);
    }
};

#endif
