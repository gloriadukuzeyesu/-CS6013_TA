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


class ConcurrentQueue {

public:
   ConcurrentQueue() :
      head_( new Node{ T{}, nullptr } ), size_( 0 ) {
      tail_ = head_;
   }

   /**
    * @brief adds a new node at the tail of the linked list.
    * 
    * @param x data to be added to the new node
    */
   void enqueue( const T & x ) {
      // create a new Node
      Node *newNode = new Node {x, nullptr}; // c++11 syntax
      {
         std::unique_lock<std::mutex> tailLock(tailMutex_); //acquire a lock on the tail of the queue
         if(head_->next == nullptr) {
            head_->next = newNode; 
            tail_ = newNode;
            tail_->next = nullptr; // may not neccessary, add more test. 
            // head_->next = tail_; 
            // head->next = newNode; 
            // tail_ = newNode; 
         } else {
            tail_->next = newNode; 
            tail_ = newNode; 
           
         } 
          size_++; 

         // //std::unique_lock<std::mutex> tailLock(tailMutex_); //acquire a lock on the tail of the queue
         // tail_->next = newNode;
         // tail_ = newNode;
         // size_++;

      } // lock will be released right after being out of scope
        //lock is automatically released when the std::unique_lock object goes out of scope at the end of 
      // {
      //    std::unique_lock<std::mutex> sizeLock(tailMutex_); //acquire a lock on the tail of the queue
      //    size_++;
      // }


      
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

   bool dequeue( T * ret ) {
      // Your code goes here.
      std::unique_lock<std::mutex> headLock(headMutex); // Acquire the lock 

      // checking if the queues is empty 
      if(head_->next == nullptr) {
         headLock.unlock();
         return false; 
      }
      // the queue is not empty
      Node *newHead = head_->next; 
      *ret = newHead->data; 
      std::cout << "ret: " << *ret << "\n";
      head_->next = newHead->next; 
      headLock.unlock();
      delete newHead; 
      size_--;
      return true;  
   }

   ~ConcurrentQueue() {

      while( head_ != nullptr ) {
         Node* temp = head_->next;
         delete head_;
         head_ = temp;
      }
   }

   int size() const { return size_; }

   T getHead() const {
      if(head_->next == nullptr) {
         return 0; 
      } else {
         return head_->next->data; 
      }}
   T getTail() const { return tail_->data; }

private:

   struct Node {
      T      data;
      Node * next;
   };

   Node * head_;
   Node * tail_;
   int  size_;
   std::mutex tailMutex_;
   std::mutex headMutex;
};
