
#include "msdMalloc.h"
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>

msdMalloc::msdMalloc() {
    hashTable_ = HashTable(INITIAL_TABLE_SIZE);
}

msdMalloc::~msdMalloc() {}

void *msdMalloc::allocate(size_t bytesToAllocate) {
    //round the up the allocation memoryBlockSize_ to the next multiple of the page size

    // source: stack overflow
    size_t allocatedBlockSize = (bytesToAllocate + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

    //call mmap to allocate the memory
    void *ptr = mmap(nullptr, allocatedBlockSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        printf("Error mmap %s", strerror(errno));
        return nullptr;
    }

    // Check if the pointer is already in the hash table
    if (hashTable_.search(ptr) != nullptr) {
        int code = munmap(ptr, allocatedBlockSize);
        if (code < 0) {
            printf("Error 2mmap %s", strerror(errno));
        } else {
            perror("Pointer already exist in the HashTable");
            return nullptr;
        }
    }

    // Insert the returned pointer to the memory address and the allocation memoryBlockSize_ in the hash table
    bool isInserted = hashTable_.insert(ptr, allocatedBlockSize);
    if (!isInserted) {
        if (munmap(ptr, allocatedBlockSize) < 0) {
            printf("Error %s", strerror(errno));
            return nullptr;
        };
    }
    // If the insertion into the hash table fails,
    // it means that there is no more space available
    // in the hash table
    // to store the memory allocation details.
    return ptr;
}

void msdMalloc::deallocate(void *ptr) {
    // Remove the entry from the hash table
    HashTableEntry *entry = hashTable_.search(ptr);
    if (entry == nullptr) {
        return;ls
    }
    entry->isDeleted_ = true; // lazy delete
    hashTable_.remove(entry->memoryBlockPointer_);

    // Deallocate the memory
    int newPtr = munmap(entry->memoryBlockPointer_, entry->memoryBlockSize_);

    if (newPtr < 0) {
        printf("Error %s", strerror(errno));
    }

}

HashTable::HashTable(size_t table_size) {
    size_ = table_size;
    count_ = 0;
    size_t tableSize = table_size * sizeof(HashTableEntry);
    // Use mmap to allocate memory for the hash table
    entries_ = static_cast<HashTableEntry *> (mmap(nullptr, tableSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));

    if (entries_ == MAP_FAILED) {
        perror("mmap failed when initializing a  HashTable");
    }
    // set all entries as isDeleted_
    // none of the entries are valid (i.e., they do not contain any allocated memory blocks)
    // By setting them all to isDeleted_, it ensures that the search() function will skip over them when
    // searching for a valid entry, and only consider entries that actually contain allocated memory blocks.
    for (size_t i = 0; i < table_size; i++) {
        entries_[i].isDeleted_ = true;
    }

}

size_t HashTable::hashFunction(void *ptr) {
    // Calculate the virtual address offset size in bits
    size_t offsetSizeInBits = 0;
    for (size_t i = 1; i < sizeof(void *); i *= 2) {
        offsetSizeInBits++;
    }
    // Shift the pointer value by the offset size
    size_t hash = reinterpret_cast<size_t>(ptr) >> offsetSizeInBits;
    return hash;
}

// Insert an entry into the hash table
bool HashTable::insert(void *ptr, size_t size) {
    // when the count of valid entries is greater than the current size of the table, grow it.
    if (count_ >= size_) {
        growHashTable();
    }

    // Find the index of the empty slot using probing based on the pointer and the size of the table
    size_t index = hashFunction(ptr) % size_;

    // trying to search an empty slots in the table
    while (!entries_[index].isDeleted_) {
        if (entries_[index].memoryBlockPointer_ == ptr) {
            // This entry already exists in the hash table
            return false;
        }
        index = (index + 1) % size_; // finding the right index within the table size
    }

    // add the entry to the hash Table
    entries_[index].memoryBlockPointer_ = ptr;
    entries_[index].memoryBlockSize_ = size;
    entries_[index].isDeleted_ = false;

    count_++; // update the count in the hashTable
    return true;
}

// Remove an entry from the hash table
void HashTable::remove(void *ptr) {
    // Find the index of the entry using probing
    size_t index = hashFunction(ptr) % size_;
    while (!entries_[index].isDeleted_) {
        if (entries_[index].memoryBlockPointer_ == ptr) {
            //mark the entry as isDeleted_
            entries_[index].isDeleted_ = true;
            count_--;
            break;
        }
        index = (index + 1) % size_;
    }
}

// Find an entry in the hash table
HashTableEntry *HashTable::search(void *ptr) {
    // Find the index of the entry using probing
    size_t index = hashFunction(ptr) % size_;
    while (!entries_[index].isDeleted_) {
        if (entries_[index].memoryBlockPointer_ == ptr) {
            // Return a pointer to the entry
            return &entries_[index];
        }
        index = (index + 1) % size_;
    }
    // entry not found
    return nullptr;
}

void HashTable::growHashTable() {
    // Calculate the new memoryBlockSize_ of the hash table
    size_t newSizeOfTheNewTable = size_ * 2;

    if (newSizeOfTheNewTable > MAX_TABLE_SIZE) {
        newSizeOfTheNewTable = MAX_TABLE_SIZE;
    }

    // Allocate memory for the new hash table using mmap
    HashTableEntry *newEntriesForNewTable = static_cast<HashTableEntry *>(mmap(NULL, newSizeOfTheNewTable * sizeof(HashTableEntry),PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    if (newEntriesForNewTable == MAP_FAILED) {
        perror("mmap failed when creating a new large table");
        exit(EXIT_FAILURE);
    }


    // Set all entries as isDeleted_ in the new hash table
    // all new entries are empty and not containing any valid data yet.
    for (size_t i = 0; i < newSizeOfTheNewTable; ++i) {
        newEntriesForNewTable[i].isDeleted_ = true;
    }

    // Copy all existing valid entries into the new hash table
    for (size_t i = 0; i < size_; i++) {
        if (!entries_[i].isDeleted_) {
            size_t newIndex = hashFunction(entries_[i].memoryBlockPointer_) % newSizeOfTheNewTable;

            while (!newEntriesForNewTable[newIndex].isDeleted_) {
                newIndex = (newIndex + 1) % newSizeOfTheNewTable;
            }

            newEntriesForNewTable[newIndex].memoryBlockPointer_ = entries_[i].memoryBlockPointer_;
            newEntriesForNewTable[newIndex].memoryBlockSize_ = entries_[i].memoryBlockSize_;
            newEntriesForNewTable[newIndex].isDeleted_ = false;

        }
    }

    // Swap the new hash table with the old one
    std::swap(entries_, newEntriesForNewTable);
    size_ = newSizeOfTheNewTable;

    // Deallocate the old hash table using munmap
    if(munmap(entries_, size_ * sizeof(HashTableEntry)) == -1) {
        perror("munmap failed when deallocating memory for old hashtable");
        exit(EXIT_FAILURE);
    }
}



