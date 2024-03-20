//
// Created by Gloria Dukuzeyesu on 4/7/23.
//

#ifndef THREADSAFEQUEUE_CONCURRENTQUEUETEST_H
#define THREADSAFEQUEUE_CONCURRENTQUEUETEST_H

#include <iostream>
#include <string>
#include <vector>
#include <thread>

class ConcurrentQueueTest {
public:
    bool testQueue(int num_producers, int num_consumers, int num_ints);
};


#endif //THREADSAFEQUEUE_CONCURRENTQUEUETEST_H
