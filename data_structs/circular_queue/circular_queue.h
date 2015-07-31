/*
 * This is the implementation of a circular buffer based queue data structure. The data structure provides a fixed sized
 * queue with constant time push and pop operations (much like a linked listed) but with out runtime memory allocation
 * costs. The structure provides the ability to push items on to the queue, to "use" items from that queue and to release
 * items from the queue. All operations are O(c) time with the size of the queue. The structure also provides the ability to
 * push an array of times on to the queue which is O(n) with the size of the array.
 *
 * cbq.c
 *
 *  Created on: July 22, 2015
 *      Author: mgrosvenor
 */


#ifndef CBQ_H_
#define CBQ_H_

#include "../../types/types.h"
#include "../array/array.h"

struct ch_cbq;
typedef struct ch_cbq ch_cbq_t;

struct ch_cbq{
    ch_word size;   //Return the max number number of elements in the cbq list
    ch_word count;  //Return the actual number of elements in the cbq
    ch_word in_use; //Return the number of elements that are currently in use. in_use <= count <= size.

    // Members prefixed with "_" are nominally "private" Don't touch my privates!
    ch_word _add_next_index;
    ch_word _release_index;
    ch_word _use_next_index;

    ch_array_t* _array; //Actual CBQ storage back end
};

//Get access to an element at the front of the list. Return a pointer to the element.
void* cbq_peek_front(ch_cbq_t* this);

//Pull an element from the front of the cbq. Return 0 on success, -1 on failure.
int cbq_pop_front(ch_cbq_t* this);

//Mark an item as "used" an item from the front of the queue. This marks the item as "in use" and returns a pointer to it.
//This is useful to use the queue for in-place storage while working on producing a result.
void* cbq_use_front(ch_cbq_t* this);

//Unmark the last item as used. This is useful if an operation failed on an item marked as used.
void cbq_unuse_front(ch_cbq_t* this);

// Put an element at the back of the circular buffer queue
void* cbq_push_back(ch_cbq_t* this, void* value);

//Push back count elements the C cbq to the back cbq-list
void* cbq_push_back_carray(ch_cbq_t* this, void* ccbq, ch_word* count);

//Free the resources associated with this cbq, assumes that individual items have been freed
void cbq_delete(ch_cbq_t* this);

//Create a new circualr buffer queue structure with size queue slots in it, each with element_size storage.
ch_cbq_t* ch_cbq_new(ch_word size, ch_word element_size);

#endif // CBQ_H_
