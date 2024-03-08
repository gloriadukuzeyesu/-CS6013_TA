#include <iostream>
#include "MyMalloc.h"
#include <iomanip> // provide the std::boolalpha to print true and false instead of 1 and 0



int main(){
    // std::cout << "Hello\n"; 

    HashTable *table = new HashTable(3);
    // std::cout << table->tableSize_ << std::endl; 
    int num = 35; 
    int ptr = 58;
    int num2 = 40; 
    int num3 = 80; 
    std::cout << "Initial table size: " << table->tableSize_ << std::endl;

    bool test1 = table->insert(&num, num);
    bool test2 = table->insert(&ptr, ptr);
    bool test3 = table->insert(&num2, num2);
    bool test4 =  table->insert(&num3, num3);

    std::cout << "test1: " << std::boolalpha << test1 << std::endl;
    std::cout << "test2: " << test2 << std::endl;
    std::cout << "test3: " << test3 << std::endl;
    std::cout << "test4: " << test4 << std::endl;

    // std::cout << &num << std::endl; 
    // std::cout << num << std::endl;
    // std::cout << num << std::endl;
    //  std::cout << &num << std::endl; 
    std::cout << "Table size: " << table->tableSize_ << std::endl;
    std::cout << "Valid Entries: " << table->countOfEntries_ << std::endl;
    table->remove(&num); 
    std::cout << "Table size after removing: " << table->tableSize_ << std::endl;
    std::cout << "Valid Entries after removing : " << table->countOfEntries_ << std::endl;

    // std::cout << table->hashTableArray->size_of_memory_allocate_ << std::endl; 


}