#include <iostream>

static const size_t PAGE_SIZE = 4096; // Size of a page in bytes

struct HashTableEntry
{
    /*
    addresses or pointer to memory block
    associated size of memory allocated to that addresses
     */

    void *memoryBlockPointer_;       // address or pointer to memory block
    size_t size_of_memory_allocate_; // size of memory block
    bool isDeleted_;                 // marked true whether entry has been deleted
};

/*
Linear Probing Hash Table Class
Linear probing is a strategy that for resolving collisions or keys that map to the
same index in the hash table.
Strategy: 1/ use a hash function to find the index for a key.
          2/ if that index contains a value, use the next available index (high index)
            if you reach the end of the array, go back to the start of the array.
*/
class HashTable
{
    /*
    key: addresses
    value: associated size of memory allocated to that addresses
    */
public:
    HashTableEntry *hashTableArray; //  or I can do HashTableEntry hashTableArray[INITIAL_TABLE_SIZE]
    // HashTableEntry hashTableArray[INITIAL_TABLE_SIZE];
    size_t tableSize_; // Current size of the hash table
    size_t countOfEntries_;
    static const size_t MAX_TABLE_SIZE = 512 * PAGE_SIZE; // Maximum size_of_memory_allocate block of the hash table

public:
    HashTable() = default;  // constuctor
    ~HashTable() = default; // deconstructor
    HashTable(size_t initialSizeTable);
    bool insert(void *ptr_address, size_t sizeOfMemoryAllocated);
    size_t remove(void *ptr_address);
    size_t hashFunction(void *ptr_address);
    HashTableEntry *find(void *ptr_address);
    // TODO
    // I made find() public cz I need to used it in the allocate() and deallocate(), Ask Dave if there is an alternative of this. BCZ instructions say find() is private

private:
    // HashTableEntry *find(void *ptr_address); //
    void growhashtable();
};

class MyMalloc
{

public:
    HashTable hashTable_{};
    MyMalloc();  // constructor
    ~MyMalloc(); // Destructor
    void *allocate(size_t bytesToAllocate);
    void *deallocate(void *ptr_address);

private:
    static const size_t INITIAL_TABLE_SIZE = 4 * PAGE_SIZE; // Initial memoryBlockSize_ of the hash table
};
