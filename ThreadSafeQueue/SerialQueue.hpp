#pragma once

////////////////////////////////////////////////////////////////////////
//
// Author:
// Date:
//
// CS 6013
//
// Outline for SerialQueue class.  Fill in the missing data, comments, etc.
//
////////////////////////////////////////////////////////////////////////

template <typename T>
class SerialQueue {

public:
   SerialQueue() :
      head_( new Node{ T{}, nullptr } ), size_( 0 )
   {
      tail_ = head_;
   }

   void enqueue( const T & x ) {
      // Your code goes here.
   }

   bool dequeue( T * ret ) {
      // Your code goes here.
   }

   ~SerialQueue() {

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
