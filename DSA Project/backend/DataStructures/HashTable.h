#ifndef DSA_HASH_TABLE_H
#define DSA_HASH_TABLE_H

template <typename V>
class HashTable {
    // Entry holds a key/value pair and flags for open addressing.
    struct Entry {
        int key;
        V value;
        bool used;
        bool deleted;
        Entry() : key(0), value(), used(false), deleted(false) {}
    };

    Entry* table;
    int capacity;
    int length;

    // Compute the hash bucket index for a key.
    int hash(int key) const {
        int h = key % capacity;
        return h < 0 ? h + capacity : h;
    }

    // Resize the table and reinsert all live entries.
    void rehash() {
        int oldCapacity = capacity;
        Entry* old = table;
        capacity *= 2;
        length = 0;
        table = new Entry[capacity];
        for (int i = 0; i < oldCapacity; ++i) {
            if (old[i].used && !old[i].deleted) put(old[i].key, old[i].value);
        }
        delete[] old;
    }

public:
    // Construct a hash table with the requested capacity.
    HashTable(int cap = 64) : capacity(cap), length(0) {
        table = new Entry[capacity];
    }

    // Clean up the table storage when the hash table is destroyed.
    ~HashTable() {
        delete[] table;
    }

    // Insert or overwrite a key/value pair in the hash table.
    // If the table is too full, rehash to a larger capacity first.
    void put(int key, const V& value) {
        if ((length + 1) * 100 / capacity > 65) rehash();
        int index = hash(key);
        while (table[index].used && !table[index].deleted && table[index].key != key) {
            index = (index + 1) % capacity;
        }
        if (!table[index].used || table[index].deleted) ++length;
        table[index].key = key;
        table[index].value = value;
        table[index].used = true;
        table[index].deleted = false;
    }

    // Retrieve the value for a key; returns false if the key is not found.
    bool get(int key, V& out) const {
        int index = hash(key);
        int checked = 0;
        while (checked < capacity && table[index].used) {
            if (!table[index].deleted && table[index].key == key) {
                out = table[index].value;
                return true;
            }
            index = (index + 1) % capacity;
            ++checked;
        }
        return false;
    }

    // Check if a key exists in the table without returning the value.
    bool contains(int key) const {
        V ignored;
        return get(key, ignored);
    }

    // Mark a key as deleted if it exists in the hash table.
    // This preserves probe chains while freeing the logical slot.
    bool remove(int key) {
        int index = hash(key);
        int checked = 0;
        while (checked < capacity && table[index].used) {
            if (!table[index].deleted && table[index].key == key) {
                table[index].deleted = true;
                --length;
                return true;
            }
            index = (index + 1) % capacity;
            ++checked;
        }
        return false;
    }
};

#endif
