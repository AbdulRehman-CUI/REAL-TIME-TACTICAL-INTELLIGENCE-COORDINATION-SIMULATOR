#ifndef DSA_STACK_H
#define DSA_STACK_H

#include "DynamicArray.h"

template <typename T>
class Stack {
    DynamicArray<T> items;

public:
    // Push a value onto the top of the stack.
    void push(const T& value) {
        items.pushBack(value);
    }

    // Remove and return the value from the top of the stack.
    T pop() {
        T value = items[items.size() - 1];
        items.removeAt(items.size() - 1);
        return value;
    }

    // Return a reference to the value at the top of the stack.
    T& top() {
        return items[items.size() - 1];
    }

    // Return true if the stack contains no items.
    bool empty() const {
        return items.empty();
    }

    // Return the current number of items in the stack.
    int size() const {
        return items.size();
    }
};

#endif
