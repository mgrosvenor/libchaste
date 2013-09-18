/*
 * llist.h
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "../../types/types.h"

struct ch_llist_node;
typedef struct ch_llist_node ch_llist_node_t;

struct ch_llist_node {
    ch_llist_node_t* next;
    ch_llist_node_t* prev;
    void* value;
};

struct ch_llist;
typedef struct ch_llist ch_llist_t;

struct ch_llist{
    ch_word count;  //Return the actual number of elements in the llist

    void* first; //Pointer to the fist valid entry list. Not valid if first == end
    void* last; //Pointer to the last valid element in the list. Not valid if last == end
    void* end; //Pointer to the one element beyond the end of the valid elements in list. Do not dereference!

    // Members prefixed with "_" are nominally "private" Don't touch my privates!
   ch_word (*_cmp)(void* lhs, void* rhs); // Comparator function for find and sort operations
   ch_array_t* _array; //Actual llist storage
   ch_llist_node_t* _first;
   ch_llist_node_t* _last;
};


//Return the element at a given offset, with bounds checking [WARN: This is slow in general]
void* llist_off(ch_llist_t* this, ch_word idx);

//Step forwards by one entry
void* llist_next (ch_llist_t* this, void* ptr);
//Step backwards by one entry
void* llist_prev(ch_llist_t* this, void* ptr);
//Step forwards by amount
void* llist_forward(ch_llist_t* this, void* ptr, ch_word amount);
//Step backwards by amount
void* llist_back(ch_llist_t* this, void* ptr, ch_word amount);

// Put an element at the front of the llist list values,
void* llist_push_front(ch_llist_t* this, void* value);
// Push an element off the front of the llist list values,
void llist_pop_front(ch_llist_t* this);
// Put an element at the back of the llist values
void* llist_push_back(ch_llist_t* this, void* value);
// Push an element at the back of the llist values
void llist_pop_back(ch_llist_t* this);

// Insert an element after the element given by ptr
void* llist_insert_after(ch_llist_t* this, void* ptr, void* value);
// Insert an element before the element giver by ptr
void* llist_insert_before(ch_llist_t* this, void* ptr, void* value);
//Remove the given ptr [WARN: In general this is very expensive]
void* llist_remove(ch_llist_t* this, void* ptr);
//Free the resources associated with this llist, assumes that individual items have been freed
void llist_delete(ch_llist_t* this);

//Push back count elements the C llist to the back llist-list
void* llist_push_back_carray(ch_llist_t* this, void* cllist, ch_word count);

//Check for equality
ch_word llist_eq(ch_llist_t* this, ch_llist_t* that);
//find the given value using the comparator function
void* llist_find(ch_llist_t* this, void* begin, void* end, void* value);
//sort into order given the comparator function
void llist_sort(ch_llist_t* this);

ch_llist_t* ch_llist_new(ch_word size, ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs) );

#endif // LINKEDLIST_H_
