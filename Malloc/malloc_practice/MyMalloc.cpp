#include "MyMalloc.h"
#include <sys/mman.h> // for mmap()
#include <cstdio>

/****************************HashTable implementation********************************************/
// HashTable will be an array of entries;


/**
 * this method is used to find the index for the key. If that spot contains
 * a value, we find the next available spot(a higher index)
 * if you reach the end of the array, go back to the front
 * @brief Hash the address to find the location to place the address/size into the hash table.
 * If there is a collision, look for the next available bucket to put the entry into.
 * @return size_t
 */

size_t HashTable::hashFunction(void *ptr_address)
{
    // on a 64-bit system, the offset for the address is 12 bits. = 2 ^ 12 = 4 K 
    size_t VirtualAddressOffsetSizeInBits = 12; // 12 bits is the offset 
    size_t index  = reinterpret_cast<size_t>(ptr_address) >> VirtualAddressOffsetSizeInBits; 
    index = index % tableSize_; 
    // Notes: Linear probing, collision handling will be done in insert()
    return index;
}

/**
 * @brief Construct a new Hash Table:: Hash Table object
 *
 * @param initialSizeTable
 */
HashTable::HashTable(size_t initialSizeTable)
{
    tableSize_ = initialSizeTable;

    // initialize the array that contains entries
    size_t len = initialSizeTable * sizeof(HashTableEntry);

    hashTableArray = (HashTableEntry *) (mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    // when the MAP_ANONYMOUS is used, the contents are initialized to 0 the fd is set to -1 and offset to 0.

    /*
    On success, mmap() returns a pointer to the mapped area
    On error, the value MAP_FAILED (that is, (void *) -1) is returned,
    and errno is set to indicate the error.
    */

    if (hashTableArray == MAP_FAILED)
    {
        perror("mmap failed when initializing a  HashTable");
    }
    /*
    set all entries in the table to true. not yet deleted.
    set all entries as isDeleted_
    none of the entries are valid
    (i.e., they do not contain any allocated memory blocks)
    By setting them all to isDeleted_, it ensures that the search() function will skip over them when
    searching for a valid entry, and only consider entries that actually contain allocated memory blocks.

    */

    for (int i = 0; i < sizeof(hashTableArray); i++)
    {
        hashTableArray[i].isDeleted_ = true;
    }
}

/**
 * @brief Insert an entry into the hash table.
 * @param ptr_address Pointer to the memory address to be inserted.
 * @param allocatedMemorySize Size of the memory allocated to the address.
 * @return True if the insertion was successful, false otherwise.
 */
bool HashTable::insert(void *ptr_address, size_t allocatedMemorySize)
{
    if (countOfEntries_ >= tableSize_) // according to Dav. Grow table when it is 75% full. 
    {
        std::cout << "About to grow table\n";
        // grow the table
        growhashtable();
        std::cout << "Done growing table \n";
    }

    // Find the index (where to insert the address) of the empty slot
    // using probing based on the pointer and the size of the table
    size_t index = hashFunction(ptr_address);
    // verify if the nothing is at this index.
    // trying to search an empty slots in the table
    while (!hashTableArray[index].isDeleted_)
    {
        if (hashTableArray[index].memoryBlockPointer_ == ptr_address)
        {
            // This entry already exists in the hash table
            return false;
        }
        // incrementing the index.
        index = (index + 1) % tableSize_; // finding the right index within the table size
    }

    // add the entry to the hash Table
    hashTableArray[index].memoryBlockPointer_ = ptr_address;
    hashTableArray[index].isDeleted_ = false;
    hashTableArray[index].size_of_memory_allocate_ = allocatedMemorySize;
    countOfEntries_++; // update the count in the hashTable
    return true;
}

/**
 * @brief remove an entry from the table
 *
 */
size_t HashTable::remove(void *ptr_address)
{
    size_t index = hashFunction(ptr_address);
    while (!hashTableArray[index].isDeleted_)
    {
        if (hashTableArray[index].memoryBlockPointer_ == ptr_address)
        {
            // mark it as deleted (lazy deletion)
            hashTableArray[index].isDeleted_ = true;
            countOfEntries_--;

            return hashTableArray[index].size_of_memory_allocate_; 
            // break;
        }
        index = (index + 1) % tableSize_;
    }

    return 0; 
}
/**
 * @brief  returns the address of an entry.
 * @return HashTableEntry*
 */
HashTableEntry *HashTable::find(void *ptr_address)
{
    size_t index = hashFunction(ptr_address);
    size_t initialIndex = index; 

    while (!hashTableArray[index].isDeleted_)
    {
        // TODO This might end up in 
        if (hashTableArray[index].memoryBlockPointer_ == ptr_address)
        {
            // returns the address of an entry
            return &hashTableArray[index];
        }

        index = (index + 1) % tableSize_;

        if(initialIndex == index) {
            // break if this ends up being endless loop. Index comes back to wheere it started. 
            // so no entry found. 
            break; 
        }
    
    }

    return nullptr; // no entry found in the table
}

/**
 * @brief create a new table that is twice the size of the current table,
 * then re-hash all the current entries into the new table.
 *
 */
void ::HashTable::growhashtable()
{
    /*
    1. create the new size of the hash table which is twice the size of the current table
    2. Allocate memory for the new hash table using mmap
    3. check if mmap succeed
    4. set all entries as deleted in new hashtable. all new entries are empty and not containing any valid data yet.
    5. Copy all existing valid entries into the new hash table
    6. Swap the new hash table with the old one
    7. Deallocate the old hashtable using munmap
    */
    // 1. Calculate the new size of the hash table
    size_t new_table_size = tableSize_ * 2;

    // control the table. May or may not be needed. Ask Dave
    if (new_table_size > MAX_TABLE_SIZE)
    {
        new_table_size = MAX_TABLE_SIZE;
    }

    // 2. Allocate memory for the new hash table using mmap
    size_t len = new_table_size * sizeof(HashTableEntry);

    HashTableEntry *newHashTable = (HashTableEntry *) (mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    
    if (newHashTable == MAP_FAILED)
    {
        perror("mmap failed when creating a new large table");
        exit(EXIT_FAILURE);
    }

    // step 4 set all entries as deleted in new hashtable
    for (size_t i = 0; i < new_table_size; i++)
    {
        newHashTable[i].isDeleted_ = true;
    }

     // hashTableArray = newHashTable; result into seg error
    tableSize_ = new_table_size;

    // step5. Copy all existing valid entries into the new hash table
    // re-hash all the current entries into the new table.

    for (size_t i = 0; i < new_table_size; i++)
    {
        if (!hashTableArray[i].isDeleted_)
        {
            // copy it over
            size_t newIndex = hashFunction(hashTableArray[i].memoryBlockPointer_) % new_table_size;

            while (!newHashTable[newIndex].isDeleted_)
            {
                newIndex = (newIndex + 1) % new_table_size;
            }
            // copy things from old hashtable to new hashtable
            newHashTable[newIndex].memoryBlockPointer_ = hashTableArray[i].memoryBlockPointer_;
            newHashTable[newIndex].size_of_memory_allocate_ = hashTableArray[i].size_of_memory_allocate_;
            newHashTable[newIndex].isDeleted_ = hashTableArray[i].isDeleted_; // same as setting it to false;
        }
    }

    // swap
    std::swap(hashTableArray, newHashTable);
    // // hashTableArray = newHashTable; result into seg error
    // tableSize_ = new_table_size;

    // deallocate the old table using munmap (hashTableArray and newHashTable have swapped their content)
    int ret = munmap(newHashTable, tableSize_ * sizeof(HashTableEntry));
    if (ret == -1)
    {
        perror("munmap failed when deallocating memory for old hashtable");
        exit(EXIT_FAILURE);
    }
}



/****************************My Malloc ********************************************/

/**
 * @brief Construct a new Custom Malloc:: Custom Malloc object
 *
 */
MyMalloc::MyMalloc()
{
    hashTable_ = HashTable(INITIAL_TABLE_SIZE);
}

/**
 * @brief Destroy the Custom Malloc:: Custom Malloc object
 *
 */
MyMalloc::~MyMalloc() {}

/**
 * @brief
 *
 * @param bytesToAllocate
 */
void *MyMalloc::allocate(size_t bytesToAllocate)
{
    //round the up the allocation memoryBlockSize_ to the next multiple of the page size or alignment

    // source: stack overflow 
    size_t allocatedBlockSize = (bytesToAllocate + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    // size_t a =  bytesToAllocate + PAGE_SIZE - 1; 

    // call mmap to allocate the memory
    void *ptr = mmap(nullptr, allocatedBlockSize,  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); 
    if (ptr == MAP_FAILED) 
    {
        printf("Error mmap %s", strerror(errno));
        return nullptr;
    }
    // }

    // Insert the returned pointer to the memory address and 
    // the allocation memoryBlockSize_ in the hash table
    bool isInserted = hashTable_.insert(ptr, allocatedBlockSize);
    if(!isInserted)
    {
        if (munmap(ptr, allocatedBlockSize) < 0) {
            printf("Error %s", strerror(errno));
            return nullptr;
        };
    }
    /**
     * if the insertion into the hashtable fails, this means there is no more space available in the hashtable 
     */
    return ptr;
}

/**
 * @brief
 *
 * @param ptr_address

 */
void *MyMalloc::deallocate(void *ptr_address)
{
   size_t sizeOfMemoryRemoved  = hashTable_.remove(ptr_address); 
   // Deallocate the memory or remove the mapping 
    // int newPr = munmap(entry->memoryBlockPointer_, entry->size_of_memory_allocate_); 
    int newPr = munmap(ptr_address, sizeOfMemoryRemoved); 
    if(newPr < 0) 
    {
        printf("Error %s", strerror(errno));
    }

}
