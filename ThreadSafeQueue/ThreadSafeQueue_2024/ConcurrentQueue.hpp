#pragma once

////////////////////////////////////////////////////////////////////////
//
// Author: Gloria Dukuzeyesu
// Date: March 18
// CS 6013
//
// Outline for ConcurrentQueue class.  Fill in the missing data, comments, etc.
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
template <typename T>

class ConcurrentQueue
{

public:
   ConcurrentQueue() : head_(new Node{T{}, nullptr}), size_(0)
   {
      tail_ = head_;
   }

   /**
    * @brief adds a new node at the tail of the linked list.
    *
    * @param x data to be added to the new node
    */

   // add a node at the end of the queue
   void enqueue(const T &x)
   {

       // create a new Node
      Node *newNode = new Node {x, nullptr}; // c++11 syntax
      {
         std::unique_lock<std::mutex> tailLock(tailMutex_); // acquire a lock on the tail of the queue
         if(head_->next == nullptr) {
            // when the queue is empty. 
            head_->next = newNode; 
            tail_ = newNode; 
            // head_->next = tail_;
         } else {
            tail_->next = newNode; 
            tail_ = newNode; 
         }
         size_++;
      }
   }

   /**
    * @brief
    * removes a node from the head of the linked list,
    * and returns the data at the new head in the variable ret.
    * If the queue is empty, dequeue returns false.
    * If an element was dequeued successfully, dequeue returns true.
    * @param ret
    * @return true
    * @return false
    */

   bool dequeue(T *ret)
   {
      std::unique_lock<std::mutex> headLock(headMutex); // Acquire the lock
      // checking if the queues is empty
      if (head_->next == nullptr)
      {
         headLock.unlock();
         return false;
      } else {
         // the queue is not empty
         Node *newHead = head_->next;
         *ret = newHead->data;
         head_->next = newHead->next;
         size_--;
         headLock.unlock();
         delete newHead;
      }

      return true;
   }

   ~ConcurrentQueue()
   {

      while (head_ != nullptr)
      {
         Node *temp = head_->next;
         delete head_;
         head_ = temp;
      }
   }

   int size() const { return size_; }

   T getHead() const
   {
      if (head_->next == nullptr)
      {
         return 0;
      }
      else
      {
         return head_->next->data;
      }
   }
   T getTail() const { return tail_->data; }

private:
   struct Node
   {
      T data;
      Node *next;
   };

   Node *head_;
   Node *tail_;
   int size_;
   std::mutex tailMutex_;
   std::mutex headMutex;
};
