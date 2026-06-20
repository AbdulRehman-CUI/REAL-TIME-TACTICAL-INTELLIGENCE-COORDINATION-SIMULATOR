#ifndef DSA_HEAP_H
#define DSA_HEAP_H

#include <string>
#include "DynamicArray.h"

struct PriorityEvent {
    int id;
    int priority;
    std::string label;
    std::string location;

    PriorityEvent(int i = 0, int p = 0, const std::string& l = "", const std::string& loc = "")
        : id(i), priority(p), label(l), location(loc) {}
};

class MaxHeap {
    DynamicArray<PriorityEvent> heap;

    // Swap two heap entries by index.
    void swapItems(int a, int b) {
        PriorityEvent temp = heap[a];
        heap[a] = heap[b];
        heap[b] = temp;
    }

    // Move a node upward until the heap property is restored.
    void bubbleUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent].priority >= heap[index].priority) break;
            swapItems(parent, index);
            index = parent;
        }
    }

    // Restore the heap property by pushing an out-of-place node down.
    void heapify(int index) {
        while (true) {
            int left = index * 2 + 1;
            int right = index * 2 + 2;
            int largest = index;
            if (left < heap.size() && heap[left].priority > heap[largest].priority) largest = left;
            if (right < heap.size() && heap[right].priority > heap[largest].priority) largest = right;
            if (largest == index) break;
            swapItems(index, largest);
            index = largest;
        }
    }

public:
    // Add a new priority event to the heap.
    void push(const PriorityEvent& event) {
        heap.pushBack(event);
        bubbleUp(heap.size() - 1);
    }

    // Remove and return the highest priority event from the heap.
    PriorityEvent pop() {
        PriorityEvent top = heap[0];
        heap[0] = heap[heap.size() - 1];
        heap.removeAt(heap.size() - 1);
        if (!heap.empty()) heapify(0);
        return top;
    }

    // Update the priority of an event by id and reheapify as needed.
    bool updatePriority(int id, int nextPriority) {
        for (int i = 0; i < heap.size(); ++i) {
            if (heap[i].id == id) {
                int old = heap[i].priority;
                heap[i].priority = nextPriority;
                if (nextPriority > old) bubbleUp(i);
                else heapify(i);
                return true;
            }
        }
        return false;
    }

    // Return the highest-priority event without removing it.
    PriorityEvent peek() const {
        return heap[0];
    }

    // Return the number of elements in the heap.
    int size() const {
        return heap.size();
    }

    // Return true if the heap contains no events.
    bool empty() const {
        return heap.empty();
    }

    // Return a read-only snapshot of the heap contents.
    const DynamicArray<PriorityEvent>& snapshot() const {
        return heap;
    }
};

#endif
