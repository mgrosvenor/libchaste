/*
 * bintree.h
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */

#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include "../../types/types.h"
#include "../array/array.h"

struct ch_bintree;
typedef struct ch_bintree ch_bintree_t;


struct ch_bintree{
    ch_word count;  //Return the actual number of elements in the bintree

    void* first; //Pointer to the fist valid entry Not valid if first == end
    void* last; //Pointer to the last valid element Not valid if last == end
    void* end; //Pointer to the one element beyond the end of the valid elements. Do not dereference!

    // Members prefixed with "_" are nominally "private" Don't touch my privates!
   ch_word (*_cmp)(void* lhs, void* rhs); // Comparator function for find and sort operations
   ch_array_t* _array; //Actual bintree storage
   ch_word _array_count; //NUmber of elements currently in the bintree
};


//Return the element at a given offset, with bounds checking [In general, this is expensive]
void* bintree_off(ch_bintree_t* this, ch_word idx);

//Step forwards by one entry
void* bintree_next (ch_bintree_t* this, void* ptr);
//Step backwards by one entry
void* bintree_prev(ch_bintree_t* this, void* ptr);
//Step forwards by amount
void* bintree_forward(ch_bintree_t* this, void* ptr, ch_word amount);
//Step backwards by amount
void* bintree_back(ch_bintree_t* this, void* ptr, ch_word amount);

// Put an element into the tree
void* bintree_push(ch_bintree_t* this, void* value);
//Remove the given ptr [WARN: In general this is very expensive]
void* bintree_remove(ch_bintree_t* this, void* ptr);

//Free the resources associated with this bintree, assumes that individual items have been freed
void bintree_delete(ch_bintree_t* this);

//Push back count elements the C bintree to the back bintree-list
void* bintree_push_back_carray(ch_bintree_t* this, void* cbintree, ch_word count);

//Check for equality
ch_word bintree_eq(ch_bintree_t* this, ch_bintree_t* that);
//find the given value using the comparator function

void* bintree_find(ch_bintree_t* this, void* begin, void* end, void* value);

//sort into order given the comparator function
void bintree_sort(ch_bintree_t* this);

ch_bintree_t* ch_bintree_new(ch_word size, ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs) );

#endif // BINARYTREE_H_
