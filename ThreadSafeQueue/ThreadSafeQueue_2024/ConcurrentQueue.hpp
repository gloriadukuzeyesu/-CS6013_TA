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

template <typename T>

class ConcurrentQueue {

public:
   ConcurrentQueue() :
      head_( new Node{ T{}, nullptr } ), size_( 0 ) {
      tail_ = head_;
   }
    std::mutex tailMutex_;
    std::mutex headMutex;

   /**
    * @brief adds a new node at the tail of the linked list.
    * 
    * @param x data to be added to the new node
    */
   void enqueue( const T & x ) {
      // create a new Node
      Node *newNode = new Node {x, nullptr}; // c++11 syntax

      std::cout << "at the very beg enqueue head data: " << head_->data << "\n"; 
      {
         std::unique_lock<std::mutex> tailLock(tailMutex_); //acquire a lock on the tail of the queue
         if(head_->next == nullptr) {
            head_ = newNode; 
            tail_ = newNode; 
            head_->next = tail_;
         } else {
            tail_->next = newNode; 
            tail_ = newNode; 
         } 

      } // lock will be released right after being out of scope
        //lock is automatically released when the std::unique_lock object goes out of scope at the end of 
      size_++;
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
      if(head_ == nullptr) {
         headLock.unlock();
         return false; 
      }



       // checking if the queue is empty,
      if(head_->next == nullptr) {
         return false; 
      }

      Node *newHead = head_->next; 
      *ret = newHead->data; 
      head_ = newHead; 
      headLock.unlock();
      delete newHead; 
      size_--;


      /**
       
        std::cout << "Deb head data: " << head_->data << "\n"; 
      *ret = head_->data; // storing the data of the head in ret

      Node *temp = head_;
      head_ = head_->next; 

      // corner case. when head becomes null, tail has to be null too
      // if(head_ == nullptr) { 
      //    tail_ = nullptr; 
      // }
      headLock.unlock();
      delete temp;

        
       */
       
       
      
     
     

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

private:

   struct Node {
      T      data;
      Node * next;
   };

   Node * head_;
   Node * tail_;
   int    size_;
};
