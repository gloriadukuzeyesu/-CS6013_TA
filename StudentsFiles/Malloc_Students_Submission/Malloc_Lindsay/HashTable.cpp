//
// Created by Lindsay Haslam on 3/14/24.
//

#include "HashTable.h"
#include <sys/mman.h>
#include <unistd.h>

HashTable::HashTable(int initialCapacity) : capacity(initialCapacity), count(0) {
    table = static_cast<Entry*>(mmap(NULL, sizeof(Entry) * capacity, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    for (int i = 0; i < capacity; i++) {
        table[i] = Entry();
    }
}

HashTable::~HashTable() {
    munmap(table, capacity * sizeof(Entry));
}

void HashTable::insert(void* key, int size) {
    //Check if the table needs to grow
    if (count >= capacity * 0.75){
        grow();
    }
    //Find an index to insert the new entry by hashing the key
    int index = hash(key);
    //Linearly probe until an inactive slot is found.
    while(table[index].isActive){
        index = (index + 1) % capacity;
    }
    table[index].key = key;
    table[index].size = size;
    table[index].isActive = true;
    count++;
}

void HashTable::remove(void* key) {
    //Search for the entry with the given key
    int index = hash(key);
    while(table[index].isActive){
        if(table[index].key == key){
            table[index].isActive = false;
            count--;
            return;
        }
        //Linear probing.
        index = (index + 1) % capacity;
    }
}

int HashTable::find(void* key) {
    //Find method to get the size associated with a pointer
    int index = hash(key);
    if(table[index].isActive){
        if (table[index].key == key){
            return table[index].size;
        }
        index = (index + 1) % capacity;
    }
    //Not found
    return 0;
}

void HashTable::grow(){
    int oldCapacity = capacity;
    capacity *= 2;
    Entry* oldTable = table;
    table = static_cast<Entry*>(mmap(nullptr, sizeof(Entry) * capacity, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));

    for (int i = 0; i < capacity; i++) {
        table[i].isActive = false;
    }
    //Reset count because insert() will increment it
    count = 0;
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].isActive) {
            //Re-insert using the public insert method
            insert(oldTable[i].key, oldTable[i].size);
        }
    }
    munmap(oldTable, sizeof(Entry) * oldCapacity);
}

int HashTable::hash(void* ptr){
        const int virtualAddressOffsetSizeInBits = 14;
        return (reinterpret_cast<uintptr_t>(ptr) >> virtualAddressOffsetSizeInBits) % capacity;
}

// Additional implementations...
//For testing
int HashTable::getCount(){
    return count;
}

//For testing
int HashTable::getCapacity(){
    return capacity;
}

