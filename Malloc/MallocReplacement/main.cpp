#include <iostream>
#include "msdMalloc.h"
#include "assert.h"
#include <stdlib.h>
#include <chrono>
#include <vector>
#include "errno.h"

// In order to run this tests, you will need to uncomment them from main.
// The tests to run Benchmark will probably take a long time because of many iterations.So I'd check that last.

void testingForNonZeroSize() {
    msdMalloc allocator;
    void *block1 = allocator.allocate(100);
    assert(block1 !=
           nullptr); // returns true if it is different from null, meaning the pointer to memory has been created

    void *block2 = allocator.allocate(200);
    assert(block2 != nullptr);

    // pointer to different chuck memory should be different
    assert(block1 != block2);

    //deallocate the memory that has been allocated
    allocator.deallocate(block1);
    allocator.deallocate(block2);


    // test that the pointer is no longer in the hash Table since we called deallocator
    assert(allocator.hashTable_.search(block1) == nullptr);
    assert(allocator.hashTable_.search(block2) == nullptr);

    std::cout << "All test for testingForNonZeroSize Passed " << std::endl;
}

void testingForZeroSizeMalloc() {
    msdMalloc msdMalloc;
//    void *ptr = msdMalloc.allocate(0);
    assert(msdMalloc.allocate(0) ==
           nullptr); // can't allocate memory of 0 bytes. mmap () will fail with the length of 0.
    assert(errno == EINVAL); // Invalid argument error
    std::cout << "\ntestingForZeroSizeMalloc Passed! \n";
}

void testForAvoidanceOfCollisionInHashTAble() {
    msdMalloc allocator2;
    void *pointer1 = allocator2.allocate(3000);
    void *pointer2 = allocator2.allocate(3000);
    // to allocate() before inserting the pointer in the hashTable, I am first checking
    // if the pointer is already in the hash table, if yes I am returning nullptr and if
    //  not present, I am adding it to the table to avoid collisions in the table.

    // Testing for collision avoidance
    assert(pointer1 != nullptr);
    assert(pointer2 != nullptr);
    assert(pointer1 != pointer2);

    std::cout << "Table size: " << allocator2.hashTable_.size_ << "\n";
    std::cout << "Memory size block: " << allocator2.hashTable_.search(pointer1)->memoryBlockSize_ << "\n";

    // these pointers should be present in the table with different pointers
    HashTableEntry *hashEntry1 = allocator2.hashTable_.search(pointer1);
    assert(hashEntry1->memoryBlockPointer_ == pointer1);


    HashTableEntry *hashEntry2 = allocator2.hashTable_.search(pointer2);
    assert(hashEntry2->memoryBlockPointer_ == pointer2);


    // these pointers should be present in the hashtable and not equal to null
    assert(allocator2.hashTable_.search(pointer1) != nullptr);
    assert(allocator2.hashTable_.search(pointer2) != nullptr);


    // deallocate the memory space
    allocator2.deallocate(pointer1);
    allocator2.deallocate(pointer2);

    // the pointer should not be present in the table
    assert(allocator2.hashTable_.search(pointer1) == nullptr);
    assert(allocator2.hashTable_.search(pointer2) == nullptr);

    std::cout << "All test for testForAvoidanceOfCollisionInHashTAble passed" << std::endl;
}

void testingMyMallocAgainstInbuiltMalloc() {
    msdMalloc msdAllocator;

    void *builtinMallocP1 = malloc(1040);
    void *msdMallocP2 = msdAllocator.allocate(20000);

    assert(builtinMallocP1 != nullptr);

    assert(msdMallocP2 != nullptr);

    assert(builtinMallocP1 != msdMallocP2);

    free(builtinMallocP1);
    assert(builtinMallocP1 != nullptr);

    msdAllocator.deallocate(msdMallocP2);
    assert(msdAllocator.hashTable_.search(msdMallocP2) == nullptr);
    assert(msdMallocP2 != nullptr); // it is not null because of lazy deleting
    std::cout << "All test for testingMyMallocAgainstInbuiltMalloc passed" << std::endl;

}

void runMallocBenchmark() {
    const int pageSize = 4096; // page size used in msdMalloc
    const int maxTableSize = 512 * pageSize; // maximum table size used in msdMalloc
    const int initialTableSize = 4 * pageSize; // initial table size used in msdMalloc
    const int iterations = 100; // number of iterations per allocation size
    const int maxSize = maxTableSize - initialTableSize; // maximum allocation size

    for (int size = 1; size <= maxSize; size += pageSize) {
        double msdMallocTime = 0.0;
        double stdMallocTime = 0.0;

        for (int i = 0; i < iterations; i++) {
            //measure my msdMalloc time
            auto msdStart = std::chrono::high_resolution_clock::now();
            msdMalloc myAllocator;
            void *msdtPtr = myAllocator.allocate(size);
            myAllocator.deallocate(msdtPtr);
            auto msdEnd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = msdEnd - msdStart;
            msdMallocTime += diff.count() * 1e6;

            // Measure stdMalloc time
            auto stdStart = std::chrono::high_resolution_clock::now();
            void *stdPtr = std::malloc(size);
            std::free(stdPtr);
            auto stdEnd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> difference = stdEnd - stdStart;
            stdMallocTime += difference.count() * 1e6;
        }


        stdMallocTime /= iterations;

        msdMallocTime /= iterations;
        std::cout << size << "  " << msdMallocTime << "  " << stdMallocTime << std::endl;

//        std::cout << "\nAllocation size:  " << size << "  " << msdMallocTime << "  " << stdMallocTime << " microseconds"
//                  << std::endl;
    }
}

void testInsertAndGrowHashTable() {
    // Create a hash table with initial size 3
    HashTable table(3);
    std::cout << "Initial table Size: " << table.size_ << std::endl;
    size_t nums1 = 1;
    // Insert the first element into the table
    auto result1 = table.insert(&nums1, nums1);
    assert(result1);
    assert(table.count_ == 1);
    assert(table.size_ == 3);


    // Insert the second element into the table
    size_t nums2 = 2;
    auto result2 = table.insert(&nums2, nums2);
    assert(result2);
    assert(table.count_ == 2);
    assert(table.size_ == 3);

    // Insert the third element into the table
    size_t nums3 = 3;
    auto result3 = table.insert(&nums3, nums3);
    assert(result3);
    assert(table.count_ == 3);
    assert(table.size_ == 3);

    // Insert the fourth element into the table, which should cause the table to grow
    size_t nums4 = 4;
    auto result4 = table.insert(&nums4, nums4);
    assert(result4);
    assert(table.count_ == 4);
    assert(table.size_ == 6);

    // Print the final size of the table and
    // indicate that the test has passed. This line will be reached if all tests passed
    std::cout << "Final Table Size: " << table.size_ << std::endl;
    std::cout << "Test passed" << std::endl;
}

void testForSearchAndRemoveFromHashTable() {
    HashTable table(10);
    int a = 5, b = 90, c = 15;
    table.insert(&a, sizeof(a));
    table.insert(&b, sizeof(b));
    table.insert(&c, sizeof(c));

    //! Testing for Find()
    HashTableEntry *entry1 = table.search(&a);
    assert(entry1 != nullptr);
    assert(entry1->memoryBlockPointer_ == &a);
    assert(entry1->memoryBlockSize_ == sizeof(a));
    assert(!entry1->isDeleted_); // entry is valid and not yet deleted


    HashTableEntry *entry2 = table.search(&b);
    assert(entry2 != nullptr);
    assert(entry2->memoryBlockPointer_ == &b);
    assert(entry2->memoryBlockSize_ == sizeof(b));
    assert(!entry2->isDeleted_); // entry is valid and not yet deleted

    HashTableEntry *entry3 = table.search(&c);
    assert(entry3 != nullptr);
    assert(entry3->memoryBlockPointer_ == &c);
    assert(entry3->memoryBlockSize_ == sizeof(c));
    assert(!entry3->isDeleted_); // entry is valid and not yet deleted

    //! Testing for Remove()
    table.remove(&a);
    assert(entry1->isDeleted_); //  flag is set to true, when lazy deleted
    assert(entry1->memoryBlockPointer_ != nullptr); // lazy deleted
    assert(entry1->memoryBlockSize_ == sizeof(a)); // due to lazy deletion
    HashTableEntry *removedEntry = table.search(&a);
    assert(removedEntry == nullptr);
    table.remove(&b);
    // assert(entry2->isDeleted_); //  flag is set to true, when lazy deleted
    assert(entry2->memoryBlockPointer_ != nullptr); // lazy deleted
    assert(entry2->memoryBlockSize_ == sizeof(a)); // due to lazy deletion
    HashTableEntry *removedEntry2 = table.search(&a);
    assert(removedEntry2 == nullptr);

    table.remove(&c);
    assert(entry3->isDeleted_); //  flag is set to true, when lazy deleted
    assert(entry3->memoryBlockPointer_ != nullptr); // lazy deleted
    assert(entry3->memoryBlockSize_ == sizeof(c)); // due to lazy deletion
    HashTableEntry *removedEntry3 = table.search(&c);
    assert(removedEntry3 == nullptr);
    std::cout << "Test1\n";
    std::string name = "Gloria";
    auto resp = table.insert(&name, name.size());
    assert(resp);
    table.remove(&name);
    std::cout << "All tests for Find and Remove in HashTable Passed\n";

}

void testingTheOverlapping() {
    // creating two allocators. They are both allocating the same sized
    // chunks of memory size block in the memory
    // I am testing if these two allocators can crush or not because they are both allocating same sized chuncks of
    // size of memory
    msdMalloc allocator;
    void *ptr1_allocator1 = allocator.allocate(1000);
    void *ptr2_allocator1 = allocator.allocate(1001);
    void *ptr3_allocator1 = allocator.allocate(1500);
    void *ptr4_allocator1 = allocator.allocate(800);


    msdMalloc allocator2;
    void *ptr1_allocator2 = allocator.allocate(1000);
    void *ptr2_allocator2 = allocator.allocate(1001);
    void *ptr3_allocator2 = allocator.allocate(1500);
    void *ptr4_allocator2 = allocator.allocate(800);


    assert(ptr1_allocator1 != nullptr);
    assert(ptr1_allocator1 != ptr1_allocator2);
    assert(ptr2_allocator1 != ptr2_allocator2);
    assert(ptr3_allocator1 != ptr3_allocator2);
    assert(ptr4_allocator1 != ptr4_allocator2);

    allocator.deallocate(ptr1_allocator1);
    allocator.deallocate(ptr2_allocator1);
    allocator.deallocate(ptr3_allocator1);
    allocator.deallocate(ptr4_allocator1);

    allocator.deallocate(ptr1_allocator2);
    allocator.deallocate(ptr2_allocator2);
    allocator.deallocate(ptr3_allocator2);
    allocator.deallocate(ptr4_allocator2);

    // Based on the flags passed in my mmap(). I don't expect allocation to overlap.
    // the flag mmap fixed is the one
    // Source: https://man7.org/linux/man-pages/man2/mmap.2.html

    //    In other words, without MAP_FIXED the system will attempt to find an empty location in the
    //     address space if the specified address range has already been mapped by something else.

    // based on this information, my allocator will not overlap.

    int iteration = 100;
    std::vector<void *> vectorOfPointers;
    const int pageSize = 4096; // page size used in msdMalloc. Size should be multiple of pagesize
    const int maxsize = 1000;
    msdMalloc allocator3;
    for (int size = 1; size <= maxsize; size += pageSize) {
        for (int i = 0; i < iteration; i++) {
            void *ptr1 = allocator3.allocate(size);
            vectorOfPointers.push_back(ptr1);
        }
    }

    for (auto p: vectorOfPointers) {
        std::cout << p << std::endl;
        assert(p != nullptr);
    }

    std::cout << "All test passed!\n";
}

int main() {

    testingForNonZeroSize();

//    testForAvoidanceOfCollisionInHashTAble();

//    testingMyMallocAgainstInbuiltMalloc();

//    testInsertAndGrowHashTable();

//    testingForZeroSizeMalloc();

//    testForSearchAndRemoveFromHashTable();

//    testingTheOverlapping();

//    runMallocBenchmark();

    std::cout << "All Test Passed\n";
    return 0;
}
