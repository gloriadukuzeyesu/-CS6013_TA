
#include <cstdio>
#include <sys/mman.h>


static const size_t PAGE_SIZE = 4096; // Size of a page in bytes

// Hash table entry struct
struct HashTableEntry {
    void *memoryBlockPointer_; // pointer to memory block
    size_t memoryBlockSize_; // size of memory block
    bool isDeleted_;  // marked true whether entry has been deleted
};

// Probing based hash table implementation
struct HashTable {
    HashTableEntry *entries_; // Pointer to the hash table entries
    size_t size_;// Current size of the hash table
    size_t count_; // Number of valid entries in the hash table

    static const size_t MAX_TABLE_SIZE = 512 * PAGE_SIZE; // Maximum memoryBlockSize_ of the hash table

    HashTable(size_t table_size);

    HashTable() = default;

    ~HashTable() = default;

    // Hash function for pointers
    size_t hashFunction(void *ptr);

    // Insert an entry into the hash table
    bool insert(void *ptr, size_t size);

    // Remove an entry from the hash table
    void remove(void *ptr);

    // Find an entry in the hash table
    HashTableEntry *search(void *ptr);

    // Grow the hash table
    void growHashTable();
};


class msdMalloc {
public:
    // Constructor and Destructor
    msdMalloc();

    ~ msdMalloc();

    // Allocate and Deallocate functions
    void *allocate(size_t bytesToAllocate);

    void deallocate(void *ptr);

    HashTable hashTable_{};


private:
    static const size_t INITIAL_TABLE_SIZE = 4 * PAGE_SIZE; // Initial memoryBlockSize_ of the hash table

};




