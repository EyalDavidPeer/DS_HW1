#pragma once


class HashSet {
private:
    struct Node {
        int key;
        Node *next;

        Node(int k) : key(k), next(nullptr) {}
    };

    Node **table;                // Array of pointers to linked lists
    int capacity;                // Current table size
    int size;                    // Number of elements
    const double LOAD_FACTOR = 0.75;  // Load factor threshold

    // Hash function: simple modulo
    int hash(int key) const {
        return key % capacity;
    }

    // Rehash the table: double the capacity and reinsert all elements
    void rehash() {
        int oldCapacity = capacity;
        Node **oldTable = table;

        capacity *= 2;  // Double the table size
        table = new Node *[capacity]();
        size = 0;

        // Reinsert all nodes into the new table
        for (int i = 0; i < oldCapacity; ++i) {
            Node *current = oldTable[i];
            while (current) {
                insert(current->key);
                Node *temp = current;
                current = current->next;
                delete temp;
            }
        }

        // Delete the old table array
        delete[] oldTable;
    }

public:
    HashSet() : capacity(8), size(0) {  // Initial capacity = 8
        table = new Node *[capacity]();
    }

    ~HashSet() {
        for (int i = 0; i < capacity; ++i) {
            Node *current = table[i];
            while (current) {
                Node *temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    // Insert a data into the hash set
    void insert(int key) {
        if (exists(key)) { return; }  // Avoid duplicates

        if ((double)size / capacity >= LOAD_FACTOR) {
            rehash();  // Resize and rehash if load factor exceeded
        }

        int index = hash(key);
        Node *newNode = new Node(key);
        newNode->next = table[index];
        table[index] = newNode;
        ++size;
    }

    // Check if a data exists
    bool exists(int key) const {
        int index = hash(key);
        Node *current = table[index];
        while (current) {
            if (current->key == key) { return true; }
            current = current->next;
        }
        return false;
    }

    int getSize() {
        return size;
    }
};