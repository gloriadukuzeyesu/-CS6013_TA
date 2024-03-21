#include <vector>
#include "SerialQueue.h"
#include <cassert>



/**
 * test enqueue with ints
 */

void testEnqueueWithInts() {
    SerialQueue<int> queue;
    for (int i = 0; i < 5; i++) {
        queue.enqueue(i);
    }
    assert(queue.size() == 5);
    std::cout << "Test Passed\n";
}

/**
 * test enqueue with strings
 */
void testEnqueueWithStrings() {
    std::vector<std::string> vec = {"Hi", "hello", "potatoes"};
    SerialQueue<std::string> queue;
    for (const std::string &i: vec) {
        queue.enqueue(i);
    }
    assert(queue.size() == 3);
    std::cout << "Test Passed\n";
}

/**
 * test enqueue Dynamically Allocation
 */
void EnqueueDynamicallyAllocation() {
    SerialQueue<int> *serialQueue = new SerialQueue<int>();
    for (int i = 0; i < 5; i++) {
        serialQueue->enqueue(i);
    }
    assert(serialQueue->size() == 5);
    std::cout << "Test Passed\n";
}

/**
 * Testing dequeue on empty queues
 */
void testOnEmptyQueue() {
    double num;
    SerialQueue<double> q1_static;
    assert(q1_static.dequeue(&num) == false);

    double num1;
    SerialQueue<double> *q1_dy = new SerialQueue<double>();
    assert(q1_dy->dequeue(&num1) == false);
    delete q1_dy;

    char x;
    SerialQueue<char> q2_static;
    assert(q2_static.dequeue(&x) == false);

    char x1;
    SerialQueue<char> *q2_dyn = new SerialQueue<char>();
    assert(q2_dyn->dequeue(&x1) == false);
    delete q2_dyn;

    std::string str;
    SerialQueue<std::string> q3;
    assert(q3.dequeue(&str) == false);

    std::string str2;
    SerialQueue<std::string> *q3_dyn = new SerialQueue<std::string>();
    assert(q3_dyn->dequeue(&str2) == false);
    delete q3_dyn;

    SerialQueue<int> q;
    int ret;
    assert(q.dequeue(&ret) == false);

    SerialQueue<int> *q5_dyn = new SerialQueue<int>();
    int ret1;
    assert(q5_dyn->dequeue(&ret1) == false);
    delete q5_dyn;
    std::cout << "Test On Empty Queues static allocated\n";
}

/**
 * Testing enqueue dequeue
 */
void testingDequeue() {
    SerialQueue<int> queue;
    int x = 20, y = 8, z = 40;
    queue.enqueue(x);
    queue.enqueue(y);
    queue.enqueue(z);
    assert(queue.size() == 3);

    int x_ret, y_ret, z_ret;
    assert(queue.dequeue(&x_ret) == true && x_ret == x);
    assert(queue.dequeue(&y_ret) == true && y_ret == y);
    assert(queue.dequeue(&z_ret) == true && z_ret == z);

    assert(queue.size() == 0);

    //testing with chars
    std::string str = "hello";
    SerialQueue<char> *queue_dy = new SerialQueue<char>();

    //adding chars to the queue
    for (char i: str) {
        queue_dy->enqueue(i);
    }
    assert(queue_dy->size() == str.size()); // sizes are equal

    char ret1, ret2, ret3, ret4, ret5, ret6;
    assert(queue_dy->dequeue(&ret1) == true && ret1 == 'h');
    assert(queue_dy->dequeue(&ret2) == true && ret2 == 'e');
    assert(queue_dy->dequeue(&ret3) == true && ret3 == 'l');
    assert(queue_dy->dequeue(&ret4) == true && ret4 == 'l');
    assert(queue_dy->dequeue(&ret5) == true && ret5 == 'o');
    assert(queue_dy->dequeue(&ret6) == false); // trying to dequeue empty queue

    assert(queue_dy->size() != str.size());
    assert(queue_dy->size() == 0);

    delete queue_dy;
}


/**
 * testSerialQueue covering all the methods
 */
void testSerialQueue() {
    // Testing enqueue and dequeue
    SerialQueue<int> q;
    int x = 10, y = 20, z = 30;

    q.enqueue(x);
    q.enqueue(y);
    q.enqueue(z);

    int a, b, c;
    assert(q.dequeue(&a) == true && a == x);
    assert(q.dequeue(&b) == true && b == y);
    assert(q.dequeue(&c) == true && c == z);
    int d;
    assert(q.dequeue(&d) == false);

    // Testing dynamic allocation
    SerialQueue<double> *q2 = new SerialQueue<double>();
    assert(q2->size() == 0);
    delete q2;

    // Testing static allocation
    SerialQueue<char> q3;
    assert(q3.size() == 0);

    // Testing dequeue on empty queue
    int ret;
    assert(q.dequeue(&ret) == false);

    // Testing destructor
    SerialQueue<std::string> *q4 = new SerialQueue<std::string>();
    q4->enqueue("Hello");
    q4->enqueue("World");
    q4->enqueue("!");
    assert(q4->size() == 3);
    delete q4;
    assert(q4->size() == 0);
    std::cout << "Testing Passed\n";
}


/**
 * please uncomment the code below to run the SerialQueue test
 */
int main() {
    testEnqueueWithInts();
    testEnqueueWithStrings();
    EnqueueDynamicallyAllocation();
    testSerialQueue();
    testOnEmptyQueue();
    testingDequeue();
}
