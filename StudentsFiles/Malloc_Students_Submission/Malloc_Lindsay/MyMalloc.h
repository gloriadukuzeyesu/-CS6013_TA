//
// Created by Lindsay Haslam on 3/14/24.
//

#ifndef MALLOC_MYMALLOC_H
#define MALLOC_MYMALLOC_H
#include "HashTable.h"

class MyMalloc {

private:
    HashTable hashTable;

public:
    MyMalloc(int initialCapacity = 64);
    void *allocate(int size);
    void deallocate(void *ptr);
    ~MyMalloc();

};



#endif //MALLOC_MYMALLOC_H
