#pragma once

#include "iostream"

template<typename T>
class SerialQueue {
private:
    struct Node {
        T data;
        Node *next;
    };

    Node *head;
    Node *tail;
    int size_;

public:
    // constructor
    SerialQueue() : head(new Node{T{}, nullptr}), size_(0) {
        tail = head;
    }

    // add a node at the end of the queue
    void enqueue(const T &x) {
        // create a new Node
        Node *newNode = new Node{x, nullptr};
        tail->next = newNode;
        tail = newNode;
        size_++;
    }

    //remove the node from the front of the queue
    // T *ret  It is a pointer to a memory location where
//        the data stored in the node being dequeued should be copied to.

    bool dequeue(T *ret) {
        if(head->next == nullptr) {
            return false; //the queue is still empty
        }
        Node *tempNode = head->next;

        *ret = tempNode->data;
        head->next = tempNode->next;
        delete tempNode;
        size_--;
        return true;
    }

    ~SerialQueue() {
        while (head) {
            Node *temp = head->next;
            delete head;
            head = temp;
        }
        size_ = 0;
    }

    int size() const { return size_; }
};



