// CamIO 2: array.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk)
// Licensed under BSD 3 Clause, please see LICENSE for more details.

#ifndef ARRAY_H_
#define ARRAY_H_

#include "../../types/types.h"

struct ch_array;
typedef struct ch_array ch_array_t;


struct ch_array{
    ch_word size;  //Return the max number number of elements in the array list

    void* first; //Pointer to the fist valid entry list. Not valid if first == end
    void* last; //Pointer to the last valid element in the array. Not valid if last == end
    void* end; //Pointer to the one element beyond the end of the valid elements in array. Do not dereference!

     // Members prefixed with "_" are nominally "private" Don't touch my privates!
    int (*_cmp)(const void *, const void *); // Comparator function for find and sort operations
    void* _array_backing; //Actual array storage
    ch_word _array_backing_size; //Number of elements allocated in the given array
    ch_word _element_size;

};


 //Resize the array
void array_resize(ch_array_t* this, ch_word new_size);

//Check for equality
ch_word array_eq(ch_array_t* this, ch_array_t* that);

//Return the element at a given offset, with bounds checking
void*  array_off(ch_array_t* this, ch_word idx);

//Step forwards by one entry
void* array_next(ch_array_t* this, void* ptr);

//Step backwards by one entry
void* array_prev(ch_array_t* this, void* ptr);

//Step forwards by amount
void* array_forward(ch_array_t* this, void* ptr, ch_word amount);

//Step backwards by amount
void* array_back(ch_array_t* this, void* ptr, ch_word amount);

//find the given value using the comparitor function
void* array_find(ch_array_t* this, void* begin, void* end, void* value);

//return the offset/index of the given item
int array_get_idx(ch_array_t* this, void* value);

//sort into order given the comparitor function
void array_sort(ch_array_t* this);

//sort into reverse order given the comparitor function
void array_sort_reverse(ch_array_t* this);

//Free the resources associated with this array, assumes that individual items have been freed
void array_delete(ch_array_t* this);

//Set at most count elements to the value in carray starting at offset in this array
void* array_from_carray(ch_array_t* this, void* carray, ch_word count);


ch_array_t* ch_array_new(ch_word size, ch_word element_size, int(*cmp)(const void* lhs, const void* rhs));

#endif //ARRAY_H_
