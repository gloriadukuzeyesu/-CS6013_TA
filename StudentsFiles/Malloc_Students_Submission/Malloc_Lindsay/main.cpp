//Created by Lindsay Haslam
//CS6013
//Malloc Assignment
//The purpose of the assignment is to compare a custom 'malloc' with the system's 'malloc'
//What is a "lazy delete" from a hash table? It's a strategy used to mark entries as "deleted"
//without actually removing things immediately. This is meant to decrease the amount of hashing.


#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include "MyMalloc.h"

//TESTS
#include <chrono>
#include <vector>

using namespace std;

void testSmallObjectAllocation(MyMalloc& allocator) {
    std::vector<void*> pointers;
    //Number of small objects
    const int numAllocations = 10000;
    //Size of each small object
    const int smallSize = 128;

    auto startAlloc = std::chrono::high_resolution_clock::now();
    //Allocate small objects
    for (int i = 0; i < numAllocations; ++i) {
        void* ptr = allocator.allocate(smallSize);
        if (ptr) {
            //Manipulate data
            memset(ptr, 0xab, smallSize);
            pointers.push_back(ptr);
        }
    }

    auto endAlloc = std::chrono::high_resolution_clock::now();
    auto startDealloc = std::chrono::high_resolution_clock::now();

    //Verify and deallocate
    for (void* ptr : pointers) {
        allocator.deallocate(ptr);
    }

    auto endDealloc = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> allocTime = endAlloc - startAlloc;
    std::chrono::duration<double, std::milli> deallocTime = endDealloc - startDealloc;
    std::cout << "Small Object Allocation time: " << allocTime.count() << " ms\n";
    std::cout << "Small Object Deallocation time: " << deallocTime.count() << " ms\n";
}

void testSmallObjectDataIntegrity(MyMalloc& allocator) {
    std::vector<void*> firstBatchPointers;
    std::vector<void*> secondBatchPointers;
    //Number of small objects in the first batch
    const int numFirstBatchAllocations = 5000;
    //Number of small objects in the second batch
    const int numSecondBatchAllocations = 3000;
    //Size of each small object
    const int smallSize = 128;
    //Data pattern for first batch
    const unsigned char firstBatchPattern = 0xAB;
    //Data pattern for second batch
    const unsigned char secondBatchPattern = 0xCD;

    //Allocate and manipulate first batch of small objects
    for (int i = 0; i < numFirstBatchAllocations; ++i) {
        void* ptr = allocator.allocate(smallSize);
        if (ptr) {
            memset(ptr, firstBatchPattern, smallSize);
            firstBatchPointers.push_back(ptr);
        }
    }

    //Allocate and manipulate second batch of small objects
    for (int i = 0; i < numSecondBatchAllocations; ++i) {
        void* ptr = allocator.allocate(smallSize);
        if (ptr) {
            memset(ptr, secondBatchPattern, smallSize);
            secondBatchPointers.push_back(ptr);
        }
    }

    //Verify data in first batch of small objects remains valid
    for (void* ptr : firstBatchPointers) {
        unsigned char* data = reinterpret_cast<unsigned char*>(ptr);
        for (int i = 0; i < smallSize; ++i) {
            if (data[i] != firstBatchPattern) {
                std::cout << "Data corruption detected in first batch of small objects." << std::endl;
                break;
            }
        }
        allocator.deallocate(ptr);
    }

    //Verify data in second batch of small objects remains valid
    for (void* ptr : secondBatchPointers) {
        unsigned char* data = reinterpret_cast<unsigned char*>(ptr);
        for (int i = 0; i < smallSize; ++i) {
            if (data[i] != secondBatchPattern) {
                std::cout << "Data corruption detected in second batch of small objects." << std::endl;
                break;
            }
        }
        allocator.deallocate(ptr);
    }

    std::cout << "Small Object Data Integrity Test Completed." << std::endl;
}

void testLargeObjectAllocation(MyMalloc& allocator) {
    std::vector<void*> pointers;
    //Number of large objects
    const int numAllocations = 100;
    //Size of each large object (1MB)
    const int largeSize = 1024 * 1024;

    auto startAlloc = std::chrono::high_resolution_clock::now();
    //Allocate large objects
    for (int i = 0; i < numAllocations; ++i) {
        void* ptr = allocator.allocate(largeSize);
        if (ptr) {
            //Manipulate data
            memset(ptr, 0xcd, largeSize);
            pointers.push_back(ptr);
        }
    }
    auto endAlloc = std::chrono::high_resolution_clock::now();

    auto startDealloc = std::chrono::high_resolution_clock::now();

    //Verify and deallocate
    for (void* ptr : pointers) {
        allocator.deallocate(ptr);
    }
    auto endDealloc = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> allocTime = endAlloc - startAlloc;
    std::chrono::duration<double, std::milli> deallocTime = endDealloc - startDealloc;
    std::cout << "Large Object Allocation time: " << allocTime.count() << " ms\n";
    std::cout << "Large Object Deallocation time: " << deallocTime.count() << " ms\n";
}

void testLargeObjectDataIntegrity(MyMalloc& allocator) {
    std::vector<void*> firstBatchPointers;
    std::vector<void*> secondBatchPointers;
    //Number of large objects in the first batch
    const int numFirstBatchAllocations = 10;
    //Number of large objects in the second batch
    const int numSecondBatchAllocations = 5;
    //Size of each large object
    const int largeSize = 1024 * 1024;
    //Data pattern for first batch
    const unsigned char firstBatchPattern = 0xAB;
    //Data pattern for second batch
    const unsigned char secondBatchPattern = 0xCD;

    //Allocate and manipulate first batch of large objects
    for (int i = 0; i < numFirstBatchAllocations; ++i) {
        void* ptr = allocator.allocate(largeSize);
        if (ptr) {
            memset(ptr, firstBatchPattern, largeSize);
            firstBatchPointers.push_back(ptr);
        }
    }

    //Allocate and manipulate second batch of large objects
    for (int i = 0; i < numSecondBatchAllocations; ++i) {
        void* ptr = allocator.allocate(largeSize);
        if (ptr) {
            memset(ptr, secondBatchPattern, largeSize);
            secondBatchPointers.push_back(ptr);
        }
    }

    //Verify data in first batch of large objects remains valid
    for (void* ptr : firstBatchPointers) {
        unsigned char* data = reinterpret_cast<unsigned char*>(ptr);
        for (int i = 0; i < largeSize; ++i) {
            if (data[i] != firstBatchPattern) {
                std::cout << "Data corruption detected in first batch of large objects." << std::endl;
                break;
            }
        }
        allocator.deallocate(ptr);
    }

    // Verify data in second batch of small objects remains valid
    for (void* ptr : secondBatchPointers) {
        unsigned char* data = reinterpret_cast<unsigned char*>(ptr);
        for (int i = 0; i < largeSize; ++i) {
            if (data[i] != secondBatchPattern) {
                std::cout << "Data corruption detected in second batch of large objects." << std::endl;
                break;
            }
        }
        allocator.deallocate(ptr);
    }

    std::cout << "Large Object Data Integrity Test Completed." << std::endl;
}

void timeAllocations() {
    const int numAllocations = 1000;
    //1KB per allocation
    const int size = 1024;
    std::vector<void*> pointers;

    //Time MyMalloc
    auto start = std::chrono::high_resolution_clock::now();

    MyMalloc myAllocator;
    for (int i = 0; i < numAllocations; ++i) {
        pointers.push_back(myAllocator.allocate(size));
    }
    for (void* ptr : pointers) {
        myAllocator.deallocate(ptr);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> myMallocTime = end - start;
    std::cout << "MyMalloc time: " << myMallocTime.count() << " ms\n";

    //Reset and time system malloc/free
    pointers.clear();
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numAllocations; ++i) {
        pointers.push_back(malloc(size));
    }
    for (void* ptr : pointers) {
        free(ptr);
    }

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> systemMallocTime = end - start;
    std::cout << "System malloc/free time: " << systemMallocTime.count() << " ms\n";
}

void testRemove() {
    HashTable table(10);
    int firstKey = 1;
    size_t size = 50;
    table.insert(&firstKey, size);
    assert(table.getCount() == 1 && "testRemove insert1");
    table.remove(&firstKey);
    assert(table.getCount() == 0 && "testRemove remove1");

    //Testing removing it again and it won't make the count -1
    table.remove(&firstKey);
    assert(table.getCount() == 0 && "testRemove remove1");

    cout << "testRemove(): Passed!\n";
}

void testInsert() {

    //Creating a table with a small capacity to make sure grow is called
    HashTable table(3);
    int firstKey = 1;
    table.insert(&firstKey, 100);
    assert(table.getCount() == 1 && "Insert1");

    int secondKey = 2;
    table.insert(&secondKey, 1);
    assert(table.getCount() == 2 && "Insert2");

    int thirdKey = 3;
    table.insert(&thirdKey, 50);
    assert(table.getCount() == 3 && "Insert3");
    assert(table.getCapacity() == 3 && "Capacity before grow()");

    //The table is expected to grow a fourth elem is inserted.
    int fourthKey = 4;
    table.insert(&fourthKey, 4);
    assert(table.getCount() == 4 && "Insert4");
    assert(table.getCapacity() == 6 && "Capacity after grow()");

    cout << "testInsert(): Passed!\n";
}



int main() {
    MyMalloc allocator;
    testSmallObjectAllocation(allocator);
    testSmallObjectDataIntegrity(allocator);
    testLargeObjectAllocation(allocator);
    testLargeObjectDataIntegrity(allocator);
    timeAllocations();
    testRemove();
    testInsert();
    return 0;
}
