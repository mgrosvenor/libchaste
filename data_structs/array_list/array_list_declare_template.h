// CamIO 2: array.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef ARRAY_LIST_DECLARE_TEMPLATE_H_
#define ARRAY_LIST_DECLARE_TEMPLATE_H_

#include "../../types/types.h"


#define declare_ch_array_list(TYPE) \
\
struct ch_array_##TYPE;\
typedef struct ch_array_list_##TYPE ch_array_list_##TYPE##_t;\
\
\
struct ch_array_list_##TYPE{\
    ch_word size;  /*Return the max number number of elements in the array list*/\
    ch_word count;  /*Return the actual number of elements in the array*/\
    void (*resize)(ch_array_list_##TYPE##_t* this, ch_word new_size); /*Resize the array*/\
    ch_word (*eq)(ch_array_list_##TYPE##_t* this, ch_array_list_##TYPE##_t* that); /*Check for equality*/\
\
    TYPE* (*off)(ch_array_list_##TYPE##_t* this, ch_word idx); /*Return the element at a given offset, with bounds checking*/\
    TYPE* first; /*Pointer to the fist valid entry list. Not valid if first == end*/\
    TYPE* last; /*Pointer to the last valid element in the list. Not valid if last == end*/\
    TYPE* end; /*Pointer to the one element beyond the end of the valid elements in list. Do not dereference! */\
\
    TYPE* (*next)(ch_array_list_##TYPE##_t* this, TYPE* ptr);  /*Step forwards by one entry*/ \
    TYPE* (*prev)(ch_array_list_##TYPE##_t* this, TYPE* ptr); /*Step backwards by one entry*/\
    TYPE* (*forward)(ch_array_list_##TYPE##_t* this, TYPE* ptr, ch_word amount);  /*Step forwards by amount*/ \
    TYPE* (*back)(ch_array_list_##TYPE##_t* this, TYPE* ptr, ch_word amount);  /*Step backwards by amount*/ \
\
    TYPE* (*find)(ch_array_list_##TYPE##_t* this, TYPE* begin, TYPE* end, TYPE value); /*find the given value using the comparitor function*/\
    void (*sort)(ch_array_list_##TYPE##_t* this); /*sort into order given the comparitor function*/\
    void (*sort_reverse)(ch_array_list_##TYPE##_t* this); /*sort into reverse order given the comparitor function*/\
\
    TYPE* (*push_front)(ch_array_list_##TYPE##_t* this, TYPE value); /* Put an element at the front of the array list values, [WARN: In general this is very expensive for an array] */\
    TYPE* (*push_back)(ch_array_list_##TYPE##_t* this, TYPE value); /* Put an elemenet at the back of the arary values*/\
    TYPE* (*insert_after)(ch_array_list_##TYPE##_t* this, TYPE* ptr, TYPE value); /* Insert an element after the element given by ptr*/\
    TYPE* (*insert_before)(ch_array_list_##TYPE##_t* this, TYPE* ptr, TYPE value); /* Insert an element before the element giver by ptr [WARN: In general this is very expensive for an array] */\
    TYPE* (*remove)(ch_array_list_##TYPE##_t* this, TYPE* ptr); /*Remove the given ptr [WARN: In general this is very expensive] */\
\
    void (*delete)(ch_array_list_##TYPE##_t* this); /*Free the resources associated with this array, assumes that individual items have been freed*/ \
\
    TYPE* (*push_back_carray)(ch_array_list_##TYPE##_t* this, TYPE* carray, ch_word count); /*Push back count elements the C array to the back array-list*/\
\
     /* Members prefixed with "_" are nominally "private" Don't touch my privates!*/\
    ch_word (*_cmp)(TYPE lhs, TYPE rhs); /* Comparator function for find and sort operations */\
    TYPE* _array_backing; /*Actual array storage*/\
    ch_word _array_backing_size; /*Number of elements allocated in the given array*/\
    ch_word _array_backing_count; /*NUmber of elements currently in the array*/\
\
};\
\
\
ch_array_list_##TYPE##_t* ch_array_list_##TYPE##_new(ch_word size, ch_word(*cmp)(TYPE lhs, TYPE rhs) )\



#endif /* ARRAY_LIST_DECLARE_H_ */
