#include <iostream>
#include "MyMalloc.h"
#include <iomanip> // provide the std::boolalpha to print true and false instead of 1 and 0

void ta_hastable_test()
{
    LinearProbingHashTable table;
    int num = 35;
    int ptr = 58;
    int num2 = 40;
    int num3 = 80;

    table.insert(&num, num);
    table.insert(&ptr, ptr);
    std::cout << "Table size2: " << table.getSize() << std::endl;

    table.insert(&num2, num2);
    table.insert(&num3, num3);
    std::cout << "Table size4: " << table.getSize() << std::endl;

    table.remove(&num);
    std::cout << "Table size3: " << table.getSize() << std::endl;

    std::cout << "Test passed\n";
}


void testingForZeroSizeMalloc() {
    MyMalloc msdMalloc;
//    void *ptr = msdMalloc.allocate(0);
    assert(msdMalloc.allocate(0) == nullptr); // can't allocate memory of 0 bytes. mmap () will fail with the length of 0.
    assert(errno == EINVAL); // Invalid argument error
    std::cout << "\ntestingForZeroSizeMalloc Passed! \n";
}

void testForAvoidanceOfCollisionInHashTAble() {
    MyMalloc allocator2;
    void *pointer1 = allocator2.allocate(3000);
    void *pointer2 = allocator2.allocate(3000);
    // to allocate() before inserting the pointer in the hashTable, I am first checking
    // if the pointer is already in the hash table, if yes I am returning nullptr and if
    //  not present, I am adding it to the table to avoid collisions in the table.

    // Testing for collision avoidance
    assert(pointer1 != nullptr);
    assert(pointer2 != nullptr);
    assert(pointer1 != pointer2);

    allocator2.deallocate(pointer1); 
    allocator2.deallocate(pointer2); 

     // the pointer should not be present in the table
    assert(allocator2.hashTable.find(pointer1) == 0);
    assert(allocator2.hashTable.find(pointer2) == 0);

}

/**


void ta_hastable_test()
{
    LinearProbingHashTable table(2);
    int num = 35;
    int ptr = 58;
    int num2 = 40;
    int num3 = 80;

    table.insert(&num, num); // 1/2 0.5
    table.insert(&ptr, ptr); // 

    
    std::cout << "Table size2: " << table.getCount() << std::endl;

    table.insert(&num2, num2);
    // table.insert(&num3, num3);
    // std::cout << "Table size4: " << table.getCount() << std::endl;

    // table.remove(&num);
    // std::cout << "Table size3: " << table.getCount() << std::endl;

    std::cout << "Test passed\n";
}


void testingForZeroSizeMalloc() {
    MyMalloc msdMalloc(20);
//    void *ptr = msdMalloc.allocate(0);
    assert(msdMalloc.allocate(0) == nullptr); // can't allocate memory of 0 bytes. mmap () will fail with the length of 0.
    assert(errno == EINVAL); // Invalid argument error
    std::cout << "\ntestingForZeroSizeMalloc Passed! \n";
}

void testForAvoidanceOfCollisionInHashTAble() {
    MyMalloc allocator2(20);
    void *pointer1 = allocator2.allocate(3000);
    void *pointer2 = allocator2.allocate(3000);
    // to allocate() before inserting the pointer in the hashTable, I am first checking
    // if the pointer is already in the hash table, if yes I am returning nullptr and if
    //  not present, I am adding it to the table to avoid collisions in the table.

    // Testing for collision avoidance
    assert(pointer1 != nullptr);
    assert(pointer2 != nullptr);
    assert(pointer1 != pointer2);

    allocator2.deallocate(pointer1); 
    allocator2.deallocate(pointer2); 

     // the pointer should not be present in the table
    assert(allocator2.allocations.find(pointer1) == -1);
    assert(allocator2.allocations.find(pointer2) == -1);

}



 */





int main()
{
    // std::cout << "Hello\n";

    HashTable table(3);

    // std::cout << table.tableSize_ << std::endl;
    int num = 35;
    int ptr = 58;
    int num2 = 40;
    int num3 = 80;
    std::cout << "Initial table size: " << table.tableSize_ << std::endl;

    bool test1 = table.insert(&num, num);
    bool test2 = table.insert(&ptr, ptr);
    bool test3 = table.insert(&num2, num2);
    bool test4 = table.insert(&num3, num3);

    std::cout << "test1: " << std::boolalpha << test1 << std::endl;
    std::cout << "test2: " << test2 << std::endl;
    std::cout << "test3: " << test3 << std::endl;
    std::cout << "test4: " << test4 << std::endl;

    // std::cout << &num << std::endl;
    // std::cout << num << std::endl;
    // std::cout << num << std::endl;
    //  std::cout << &num << std::endl;
    std::cout << "Table size: " << table.tableSize_ << std::endl;
    std::cout << "Valid Entries: " << table.countOfEntries_ << std::endl;
    table.remove(&num);
    std::cout << "Table size after removing: " << table.tableSize_ << std::endl;
    std::cout << "Valid Entries after removing : " << table.countOfEntries_ << std::endl;

    // std::cout << table.hashTableArray->size_of_memory_allocate_ << std::endl;

    HashTable table2(20);
    bool test8 = table2.insert(&num, num);
    bool test5 = table2.insert(&ptr, ptr);
    bool test6 = table2.insert(&num2, num2);
    bool test7 = table2.insert(&num3, num3);
    std::cout << "test5: " << test5 << std::endl;
    std::cout << "test6: " << test6 << std::endl;
    std::cout << "test7: " << test7 << std::endl;
    std::cout << "test8: " << test8 << std::endl;
}