#ifndef DSA_LINKED_LIST_H
#define DSA_LINKED_LIST_H

template <typename T>
class LinkedList {
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int length;

public:
    // Initialize an empty singly linked list.
    LinkedList() : head(nullptr), tail(nullptr), length(0) {}

    // Clean up all allocated list nodes.
    ~LinkedList() {
        clear();
    }

    // Append a value to the end of the list.
    void pushBack(const T& value) {
        Node* node = new Node(value);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++length;
    }

    // Remove the first node that matches the value and return whether it was removed.
    bool removeFirst(const T& value) {
        Node* prev = nullptr;
        Node* cur = head;
        while (cur) {
            if (cur->value == value) {
                if (prev) prev->next = cur->next;
                else head = cur->next;
                if (cur == tail) tail = prev;
                delete cur;
                --length;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // Remove all nodes from the list and reset it to empty.
    void clear() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = tail = nullptr;
        length = 0;
    }

    // Return the number of elements currently stored.
    int size() const {
        return length;
    }
};

#endif
