#ifndef DSA_QUEUE_H
#define DSA_QUEUE_H

template <typename T>
class Queue {
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };

    Node* frontNode;
    Node* backNode;
    int length;

public:
    // Initialize an empty FIFO queue.
    Queue() : frontNode(nullptr), backNode(nullptr), length(0) {}

    // Clean up all nodes when the queue is destroyed.
    ~Queue() {
        while (!empty()) dequeue();
    }

    // Add a value to the back of the queue.
    void enqueue(const T& value) {
        Node* node = new Node(value);
        if (!backNode) {
            frontNode = backNode = node;
        } else {
            backNode->next = node;
            backNode = node;
        }
        ++length;
    }

    // Remove and return the value from the front of the queue.
    T dequeue() {
        Node* node = frontNode;
        T value = node->value;
        frontNode = frontNode->next;
        if (!frontNode) backNode = nullptr;
        delete node;
        --length;
        return value;
    }

    // Return a reference to the value at the front without removing it.
    T& front() {
        return frontNode->value;
    }

    // Return true if the queue has no elements.
    bool empty() const {
        return length == 0;
    }

    // Return the number of values currently stored in the queue.
    int size() const {
        return length;
    }
};

#endif
