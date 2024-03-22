#include "iostream"
#include "SerialQueue.hpp"
#include <cassert>
#include <vector>

// clang++ -Wall -std=c++11  SerialQueueTests.cpp -o pro

void staticAllocation_enqueue()
{
    // Ints
    SerialQueue<int> queue;
    for (int i = 0; i < 5; i++)
    {
        queue.enqueue(i);
    }
    assert(queue.size() == 5);

    // std::string
    std::vector<std::string> list = {"foo", "doe"};
    SerialQueue<std::string> queue_strings;
    for (std::string &str : list)
    { // accecc by reference
        queue_strings.enqueue(str);
    }
    assert(queue_strings.size() == 2);
    std::cout << "Static allocation enqueue passed\n";
}

void dynamicAllocation_enqueue()
{
    // ints
    SerialQueue<int> *queue = new SerialQueue<int>();
    for (int i = 0; i < 5; i++)
    {
        queue->enqueue(i);
    }
    assert(queue->size() == 5);

    // string
    std::vector<std::string> list = {"foo", "doe"};
    SerialQueue<std::string> *queue_strings = new SerialQueue<std::string>();
    for (std::string &str : list)
    { // accecc by reference
        queue_strings->enqueue(str);
    }
    assert(queue_strings->size() == 2);
    std::cout << "Dynamic allocation enqueue passed\n";
}



void staticAllocation_deque() {

    SerialQueue<int> queue;    
    int x = 20, y = 8, z = 40;
    queue.enqueue(x);
    queue.enqueue(y);
    queue.enqueue(z);
    assert(queue.size() == 3);
    std::cout << "Head Data: " << queue.getHeadData() << "\n"; 
    std::cout <<  "Tail Data: " << queue.getTailData() << "\n"; 

    int x_ret, y_ret, z_ret; 
    assert(queue.dequeue(&x_ret) == true && x_ret == y); 
    std::cout << "Head Data: " << queue.getHeadData() << "\n";
        std::cout <<  "Tail Data: " << queue.getTailData() << "\n"; 
 
    assert(queue.dequeue(&y_ret) == true && y_ret == 40);
    // assert(queue.dequeue(&z_ret) == true && z_ret == z);

    // std::cout << "Head Data: " << queue.getHeadData() << "\n"; 
    // std::cout << "Head Data: " << queue.getHeadData() << "\n"; 
    // std::cout <<  "Tail Data: " << queue.getTailData() << "\n"; 

    // assert(queue.dequeue(&z_ret) == true && z_ret == z);

    std::cout << "Head Data: " << queue.getHeadData() << "\n"; 
    std::cout <<  "Tail Data: " << queue.getTailData() << "\n"; 

    /**

    assert(queue.dequeue(&z_ret) == false);
    std::cout << "z_ret: " << z_ret << "\n";
     std::cout << "size: " << queue.size() << "\n";

    assert(queue.size() == 0);

    int val;
    assert(queue.dequeue(&val) == false); // trying to an empty queue

     */
}



void testingOneEmptyList() {
    SerialQueue<int> queue;    
    int x = 20; 
    int y = 8; 
    int z = 40;
    queue.enqueue(x);
    queue.enqueue(y);
    queue.enqueue(z);
    assert(queue.size() == 3);
    int xx; 
    std::cout << queue.dequeue(&xx); 



}
int main()
{
    //staticAllocation_enqueue();
    //dynamicAllocation_enqueue();
    // staticAllocation_deque(); // TODO needs a rework
    staticAllocation_deque(); 
    // testingOneEmptyList(); 
    std::cout << "hello\n"; 
}