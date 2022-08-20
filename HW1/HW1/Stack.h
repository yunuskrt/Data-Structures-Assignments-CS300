
//  Created by Yunus Kerestecioğlu on 26.03.2022.
//

#ifndef Stack_h
#define Stack_h

#include <stdio.h>



template <class Object>
    class Stack
    {
        public:
           Stack( );
           Stack( const Stack & rhs );
           ~Stack( );

           bool isEmpty( ) const; // returns true if there is no element
           bool isFull( ) const; // returns false
           void makeEmpty( ); // makes stack empty

           void pop( ); // deletes element in the top
           void push( const Object & x ); // pushes element in the top
           Object topAndPop( ); // returns the top element which is popped
           const Object & top( ) const; // returns the top element

           const Stack & operator=( const Stack & rhs ); // deep copy of the stack
        
        private:
           struct ListNode
           {
               Object   element;
               ListNode *next;

               ListNode( const Object & theElement, ListNode * n = NULL )
                : element( theElement ), next( n ) { }
           };

           ListNode *topOfStack;  // list itself is the stack
       };


#endif /* Stack_hpp */
