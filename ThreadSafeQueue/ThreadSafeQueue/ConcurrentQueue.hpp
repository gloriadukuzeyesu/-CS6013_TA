#pragma once

#include "iostream"

template<typename T>
class ConcurrentQueue {
private:
    struct Node {
        T data;
        Node *next;

    };

    Node *head;
    Node *tail;
    std::atomic<int> size_;

public:
    // constructor
    ConcurrentQueue() : head(new Node{T{}, nullptr}), size_(0) {
        tail = head;
    }

    std::mutex tailMutex_;
    std::mutex headMutex;

    // add a node at the end of the queue
    void enqueue(const T &x) {
        // create a new Node
        Node *newNode = new Node{x, nullptr};
        {
            std::unique_lock<std::mutex> tailLock(tailMutex_); //acquire a lock on the tail of the queue
            tail->next = newNode;
            tail = newNode;
        } // lock will be released right after being out of scope
        //lock is automatically released when the std::unique_lock object goes out of scope at the end of the block.
        size_++;
    }

/**
 * remove the node from the front of the queue
 * @param ret  It is a pointer to a memory location where the data stored in the node being dequeued should be copied to.
 * @return  true if successful remove the front node and false otherwise.
 */
    bool dequeue(T *ret) {
        std::unique_lock<std::mutex> headLock(headMutex);

        if (head->next == nullptr) { // the queue is still empty
            headLock.unlock();
            return false;
        }

        Node *newHead = head->next;
        *ret = newHead->data;
        head->next = newHead->next;
        headLock.unlock();
        delete newHead;
        size_--;
        return true;
    }

    ~ConcurrentQueue() {
        while (head) {
            Node *temp = head->next;
            delete head;
            head = temp;
        }
        size_ = 0;
    }

    int size() const { return size_; }
};

