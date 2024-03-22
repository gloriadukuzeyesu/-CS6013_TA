//
// Created by Lindsay Haslam on 3/14/24.
//

#include "MyMalloc.h"
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>

MyMalloc::MyMalloc(int initialCapacity) : hashTable(initialCapacity) {}

const int initialHashTableCapacity = 64;
const int fixedPageSize = 1600;

void* MyMalloc::allocate(int size) {

    if (size == 0 ){
        return nullptr;
    }
    //Calculate adjustedSize by rounding up the requested size to the next multiple of fixedPageSize
    int adjustedSize = ((size + fixedPageSize - 1) / fixedPageSize) * fixedPageSize;
    //Allocate memory using mmap by requesting 'adjustedSize' bytes of memory.
    void* ptr = mmap(NULL, adjustedSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //Check for allocation failure
    if (ptr == MAP_FAILED) {
        return nullptr;
    }
    hashTable.insert(ptr, adjustedSize);
    //Return allocated memory address
    return ptr;
}

void MyMalloc::deallocate(void* ptr) {
    if (ptr == nullptr){
        return;
    }
    int size = hashTable.find(ptr);
    //Check that it was found
    if (size > 0){
        //Deallocate the memory
        munmap(ptr, size);
        hashTable.remove(ptr);
    }
    else{
        std::cerr << "Error in finding the key to de allocate.";
    }
}

MyMalloc::~MyMalloc(){
    // Cleanup resources if needed
}