//
// Created by Lindsay Haslam on 3/14/24.
//

#ifndef MALLOC_HASHTABLE_H
#define MALLOC_HASHTABLE_H
#include <cstddef> // for size_t

class HashTable {
private:
    struct Entry {
        void* key;
        int size;
        bool isActive;
        Entry() : key(nullptr), size(0), isActive(false) {}
    };

    Entry* table;
    int capacity;
    int count;

    int hash(void* ptr);
    void grow();

public:
    HashTable(int initialCapacity);
    ~HashTable();
    void insert(void* key, int size);
    void remove(void* key);
    int find(void* key);
    int getCount();
    int getCapacity();
};

#endif //MALLOC_HASHTABLE_H
