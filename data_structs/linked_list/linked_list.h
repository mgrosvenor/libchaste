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
};

struct ch_llist;
typedef struct ch_llist ch_llist_t;

struct ch_llist{
    ch_word count;  //Return the actual number of elements in the llist

    // Members prefixed with "_" are nominally "private" Don't touch my privates!
   cmp_void_f _cmp; // Comparator function for find and sort operations
   ch_llist_node_t* _first;
   ch_llist_node_t* _last;
   ch_word _element_size;
};

typedef struct {
    //These state variables are private
    ch_llist_node_t* _node;

    //This is public
    void* value;
} ch_llist_it;


//Return the element at a given offset, with bounds checking [WARN: This is slow in general]
ch_llist_it llist_off(ch_llist_t* this, ch_word idx);

//Get the first entry
ch_llist_it llist_first(ch_llist_t* this);
//Get the last entry
ch_llist_it llist_last(ch_llist_t* this);
//Get the end
ch_llist_it llist_end(ch_llist_t* this);

//Step forwards by one entry
void llist_next (ch_llist_t* this, ch_llist_it* it);
//Step backwards by one entry
void llist_prev(ch_llist_t* this, ch_llist_it*);
//Step forwards by amount
void llist_forward(ch_llist_t* this, ch_llist_it* it, ch_word amount);
//Step backwards by amount
void llist_back(ch_llist_t* this, ch_llist_it* it, ch_word amount);

// Put an element at the front of the llist list values,
ch_llist_it llist_push_front(ch_llist_t* this, const void* value);
// Push an element off the front of the llist list values,
ch_llist_it llist_pop_front(ch_llist_t* this);
// Put an element at the back of the llist values
ch_llist_it llist_push_back(ch_llist_t* this, const void* value);
// Push an element at the back of the llist values
ch_llist_it llist_pop_back(ch_llist_t* this);

// Insert an element after the element given by ptr
ch_llist_it* llist_insert_after(ch_llist_t* this, ch_llist_it* itr, const void* value);
// Insert an element before the element giver by ptr
ch_llist_it* llist_insert_before(ch_llist_t* this, ch_llist_it* it, const void* value);
<<<<<<< HEAD
//Remove the given element with given iterator
ch_llist_it llist_remove_it(ch_llist_t* this, ch_llist_it* itr);
//Remove all elements with given value
ch_llist_it llist_remove_all(ch_llist_t* this, const void* value);
=======
//Remove the given iterator
ch_llist_it llist_remove_it(ch_llist_t* this, ch_llist_it* itr);
//Remove all items with the given value
ch_llist_it llist_remove_all(ch_llist_t* this, void* value);
>>>>>>> mpg39

//Remove all elememts from the list
void llist_pop_all(ch_llist_t* this);

//Free the resources associated with this llist, assumes that individual items have been freed
void llist_delete(ch_llist_t* this);

//Push back count elements the C llist to the back llist-list
ch_llist_it llist_push_back_carray(ch_llist_t* this, const void* carray, ch_word count);

//Check for equality
ch_word llist_eq(ch_llist_t* this, ch_llist_t* that);
//find the given value using the comparator function
ch_llist_it llist_find(ch_llist_t* this, ch_llist_it* begin, ch_llist_it* end, void* value);
ch_llist_it llist_find_first(ch_llist_t* this, void* value);
ch_llist_it llist_find_next(ch_llist_t* this,  ch_llist_it* begin, void* value);
ch_llist_it llist_insert_inorder(ch_llist_t* this,  void* value);
ch_llist_it llist_insert_carray_ordered(ch_llist_t* this, void* carray, ch_word count);
//sort into order given the comparator function
void llist_sort(ch_llist_t* this);

ch_llist_t* ch_llist_new( ch_word element_size, cmp_void_f cmp );
ch_llist_t* ch_llist_init( ch_llist_t* this, ch_word element_size, cmp_void_f cmp );

#endif // LINKEDLIST_H_
