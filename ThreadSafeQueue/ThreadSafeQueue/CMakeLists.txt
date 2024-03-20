cmake_minimum_required(VERSION 3.24)
project(ThreadSafeQueue)

set(CMAKE_CXX_STANDARD 17)

add_executable(ThreadSafeQueue main.cpp SerialQueue.h SerialQueueTests.cpp ConcurrentQueue.hpp ConcurrentQueueTest.cpp ConcurrentQueueTest.h ConcurrentQueueTest.cpp ConcurrentQueueTest.h)
