// CamIO 2: array.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef ARRAY_DECLARE_TEMPLATE_H_
#define ARRAY_DECLARE_TEMPLATE_H_

#include "../types/types.h"


#define declare_ch_array(TYPE) \
\
struct ch_array_##TYPE;\
typedef struct ch_array_##TYPE ch_array_##TYPE##_t;\
\
\
struct ch_array_##TYPE{\
    ch_word size;  /*Return the max number number of elements in the array list*/\
\
    void (*resize)(ch_array_##TYPE##_t* this, ch_word new_size); /*Resize the array*/\
    ch_word (*eq)(ch_array_##TYPE##_t* this, ch_array_##TYPE##_t* that); /*Check for equality*/\
\
    TYPE* (*off)(ch_array_##TYPE##_t* this, ch_word idx); /*Return the element at a given offset, with bounds checking*/\
    TYPE* first; /*Pointer to the fist valid entry list. Not valid if first == end*/\
    TYPE* last; /*Pointer to the last valid element in the array. Not valid if last == end*/\
    TYPE* end; /*Pointer to the one element beyond the end of the valid elements in array. Do not dereference! */\
\
    TYPE* (*next)(ch_array_##TYPE##_t* this, TYPE* ptr);  /*Step forwards by one entry*/ \
    TYPE* (*prev)(ch_array_##TYPE##_t* this, TYPE* ptr); /*Step backwards by one entry*/\
    TYPE* (*forward)(ch_array_##TYPE##_t* this, TYPE* ptr, ch_word amount);  /*Step forwards by amount*/ \
    TYPE* (*back)(ch_array_##TYPE##_t* this, TYPE* ptr, ch_word amount);  /*Step backwards by amount*/ \
\
    TYPE* (*find)(ch_array_##TYPE##_t* this, TYPE* begin, TYPE* end, TYPE value); /*find the given value using the comparitor function*/\
    void (*sort)(ch_array_##TYPE##_t* this); /*sort into order given the comparitor function*/\
    void (*sort_reverse)(ch_array_##TYPE##_t* this); /*sort into reverse order given the comparitor function*/\
\
    TYPE* (*remove)(ch_array_##TYPE##_t* this, TYPE* ptr); /*Remove the given ptr [WARN: In general this is very expensive] */\
\
    void (*delete)(ch_array_##TYPE##_t* this); /*Free the resources associated with this array, assumes that individual items have been freed*/ \
\
    TYPE* (*from_carray)(ch_array_##TYPE##_t* this, ch_word offset, TYPE* carray, ch_word count); /*Set at most count elements to the value in carray starting at offset in this array*/\
\
     /* Members prefixed with "_" are nominally "private" Don't touch my privates!*/\
    ch_word (*_cmp)(TYPE lhs, TYPE rhs); /* Comparator function for find and sort operations */\
    TYPE* _array_backing; /*Actual array storage*/\
    ch_word _array_backing_size; /*Number of elements allocated in the given array*/\
\
};\
\
\
ch_array_##TYPE##_t* ch_array_##TYPE##_new(ch_word size, ch_word(*cmp)(TYPE lhs, TYPE rhs))



#endif /* ARRAY_DECLARE_H_ */
