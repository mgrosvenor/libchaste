// CamIO 2: array.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk)
// Licensed under BSD 3 Clause, please see LICENSE for more details.

#ifndef ARRAY_H_
#define ARRAY_H_

#include "../../types/types.h"

struct ch_array;
typedef struct ch_array ch_array_t;


struct ch_array{
    ch_word size;  /*Return the max number number of elements in the array list*/

    void (*resize)(ch_array_t* this, ch_word new_size); /*Resize the array*/
    ch_word (*eq)(ch_array_t* this, ch_array_t* that); /*Check for equality*/

    void* (*off)(ch_array_t* this, ch_word idx); /*Return the element at a given offset, with bounds checking*/
    void* first; /*Pointer to the fist valid entry list. Not valid if first == end*/
    void* last; /*Pointer to the last valid element in the array. Not valid if last == end*/
    void* end; /*Pointer to the one element beyond the end of the valid elements in array. Do not dereference! */

    void* (*next)(ch_array_t* this, void* ptr);  /*Step forwards by one entry*/ 
    void* (*prev)(ch_array_t* this, void* ptr); /*Step backwards by one entry*/
    void* (*forward)(ch_array_t* this, void* ptr, ch_word amount);  /*Step forwards by amount*/ 
    void* (*back)(ch_array_t* this, void* ptr, ch_word amount);  /*Step backwards by amount*/ 

    void* (*find)(ch_array_t* this, void* begin, void* end, void* value); /*find the given value using the comparitor function*/
    void (*sort)(ch_array_t* this); /*sort into order given the comparitor function*/
    void (*sort_reverse)(ch_array_t* this); /*sort into reverse order given the comparitor function*/

    void* (*remove)(ch_array_t* this, void* ptr); /*Remove the given ptr [WARN: In general this is very expensive] */

    void (*delete)(ch_array_t* this); /*Free the resources associated with this array, assumes that individual items have been freed*/ 

    void* (*from_carray)(ch_array_t* this, void* carray, ch_word count); /*Set at most count elements to the value in carray starting at offset in this array*/

     /* Members prefixed with "_" are nominally "private" Don't touch my privates!*/
    ch_word (*_cmp)(void* lhs, void* rhs); /* Comparator function for find and sort operations */
    ch_byte* _array_backing; /*Actual array storage*/
    ch_word _array_backing_size; /*Number of elements allocated in the given array*/
    ch_word _element_size;

};

ch_array_t* ch_array_new(ch_word size, ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs));

#endif //ARRAY_H_
