#include <iostream>
#include <vector>
#include"ConcurrentQueueTest.h"

/**
 * if you finish running the SerialQueueTests.cpp please comment its main ()
 * and uncomment the code below to run the concurrentQueueTest
 * use command clang++ -std=c++17 -o pro  main.cpp ConcurrentQueueTest.cpp
 * ./pro 10 10 100
 * you may change the value according to your wants
 * Notes :
 *         - If you add a node to a queue and then remove the same node from the queue, a test should pass.
 *           This means that the queue correctly enqueues and dequeues nodes.
 *         _ If you add fewer nodes to a queue than you remove from it, the test should fail.
 *           This is because the queue will still have some nodes remaining in it even though
 *           you have removed more nodes than you added.
 *         - If you remove more nodes from a queue than you add to it, the queue should have a size of 0.
 *           This is because you have removed more nodes than there were in the queue, so the queue should be empty.
 */
int main(int argc, char *argv[]) {
    int num_producers;
    int num_consumers;
    int num_ints;
    ConcurrentQueueTest concurrentQueueTest;
    if(argc == 4) {
        num_producers = atoi(argv[1]);
        num_consumers = atoi(argv[2]);
        num_ints = atoi(argv[3]);
        concurrentQueueTest.testQueue(num_producers, num_consumers, num_ints);
    } else {
        std::cout << "invalid input\n";
    }
}
