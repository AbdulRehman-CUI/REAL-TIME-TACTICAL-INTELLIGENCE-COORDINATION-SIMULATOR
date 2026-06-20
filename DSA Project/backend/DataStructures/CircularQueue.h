#ifndef DSA_CIRCULAR_QUEUE_H
#define DSA_CIRCULAR_QUEUE_H

template <typename T>
class CircularQueue {
    T* data;
    int capacity;
    int head;
    int tail;
    int length;

public:
    // CircularQueue holds values in a fixed-size ring buffer for FIFO access.
    CircularQueue(int cap = 32) : capacity(cap), head(0), tail(0), length(0) {
        data = new T[capacity];
    }

    // Release the storage used by the queue.
    ~CircularQueue() {
        delete[] data;
    }

    // Add a new value to the end of the circular queue if there is room.
    bool enqueue(const T& value) {
        if (length == capacity) return false;
        data[tail] = value;
        tail = (tail + 1) % capacity;
        ++length;
        return true;
    }

    // Remove the oldest value from the queue and store it into out.
    bool dequeue(T& out) {
        if (length == 0) return false;
        out = data[head];
        head = (head + 1) % capacity;
        --length;
        return true;
    }

    // Return true when the queue has no elements available.
    bool empty() const {
        return length == 0;
    }

    // Return the current number of elements stored in the queue.
    int size() const {
        return length;
    }
};

#endif
