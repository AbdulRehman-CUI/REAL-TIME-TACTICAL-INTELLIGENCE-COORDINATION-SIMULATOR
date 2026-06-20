#ifndef DSA_DYNAMIC_ARRAY_H
#define DSA_DYNAMIC_ARRAY_H

template <typename T>
class DynamicArray {
    T* data;
    int length;
    int capacity;

    // Resize the underlying array buffer to the specified capacity.
    // Existing elements are copied to the new storage.
    void resize(int nextCapacity) {
        T* next = new T[nextCapacity];
        for (int i = 0; i < length; ++i) {
            next[i] = data[i];
        }
        delete[] data;
        data = next;
        capacity = nextCapacity;
    }

public:
    // Construct a dynamic array with an initial capacity.
    DynamicArray(int initialCapacity = 8) {
        capacity = initialCapacity > 0 ? initialCapacity : 8;
        length = 0;
        data = new T[capacity];
    }

    // Copy constructor performs a deep copy of another DynamicArray.
    DynamicArray(const DynamicArray& other) {
        capacity = other.capacity;
        length = other.length;
        data = new T[capacity];
        for (int i = 0; i < length; ++i) data[i] = other.data[i];
    }

    // Assignment operator deep copies data from another DynamicArray.
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;
        delete[] data;
        capacity = other.capacity;
        length = other.length;
        data = new T[capacity];
        for (int i = 0; i < length; ++i) data[i] = other.data[i];
        return *this;
    }

    // Clean up the allocated buffer when the dynamic array is destroyed.
    ~DynamicArray() {
        delete[] data;
    }

    // Append a value to the end of the array, growing storage if needed.
    void pushBack(const T& value) {
        if (length == capacity) resize(capacity * 2);
        data[length++] = value;
    }

    // Remove the element at the given index, shifting subsequent items down.
    bool removeAt(int index) {
        if (index < 0 || index >= length) return false;
        for (int i = index; i < length - 1; ++i) data[i] = data[i + 1];
        --length;
        if (capacity > 8 && length < capacity / 4) resize(capacity / 2);
        return true;
    }

    // Clear the array while retaining the current allocated buffer.
    void clear() {
        length = 0;
    }

    // Return the current number of elements stored in the array.
    int size() const {
        return length;
    }

    // Return whether the array currently contains no elements.
    bool empty() const {
        return length == 0;
    }

    // Access the element at the given index for modification.
    T& operator[](int index) {
        return data[index];
    }

    // Access the element at the given index in a const context.
    const T& operator[](int index) const {
        return data[index];
    }
};

#endif
