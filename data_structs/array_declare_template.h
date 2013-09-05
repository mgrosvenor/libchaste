// CamIO 2: array.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef ARRAY_DECLARE_TEMPLATE_H_
#define ARRAY_DECLARE_TEMPLATE_H_

#include "../types/types.h"


#define declare_ch_array(TYPE) \
\
struct ch_array_##TYPE##_iter;\
typedef struct ch_array_##TYPE##_iter ch_array_##TYPE##_iter_t;\
\
struct ch_array;\
typedef struct ch_array_##TYPE ch_array_##TYPE##_t;\
\
struct ch_array_##TYPE##_iter {\
    TYPE* value; /* Pointer to the value that we are currently looking at. This may be NULL which indicates an invalid entry */\
    ch_array_##TYPE##_t* _priv; /* Pointer to the data structure that this is from */\
    ch_word _index; /*Index into the data structure that this is from. This may be -1 which indicates an invalid entry */\
    ch_array_##TYPE##_iter_t*(*next)(ch_array_##TYPE##_iter_t* this);  /*Step forwards by one*/ \
    ch_array_##TYPE##_iter_t*(*prev)(ch_array_##TYPE##_iter_t* this); /*Step backwards by one*/\
    ch_array_##TYPE##_iter_t*(*forward)(ch_array_##TYPE##_iter_t* this, ch_word amount);  /*Step forwards by amount*/ \
    ch_array_##TYPE##_iter_t*(*back)(ch_array_##TYPE##_iter_t* this, ch_word amount);  /*Step backwards by amount*/ \
    ch_word (*eq)(ch_array_##TYPE##_iter_t* lhs, ch_array_##TYPE##_iter_t* rhs); /*Compare iterators for equaility*/ \
    void (*delete)(ch_array_##TYPE##_iter_t* this);\
};\
\
ch_array_##TYPE##_iter_t* ch_array_int_iter_new(ch_array_##TYPE##_t* array, ch_word off);\
\
\
struct ch_array_##TYPE{\
    ch_word(*size)(ch_array_##TYPE##_t* this);  /*Return the max number number of elements in the array*/\
    ch_word(*count)(ch_array_##TYPE##_t* this);  /*Return the actual number of elements in the array*/\
    ch_word(*resize)(ch_array_##TYPE##_t* this, ch_word new_size); /*Resize the array*/\
\
    TYPE*(*off)(ch_array_##TYPE##_t* this, ch_word idx); /*Return the element at a given offset, with bounds checking*/\
\
    ch_array_##TYPE##_iter_t*(*iter_at)(ch_array_##TYPE##_t* this, ch_word idx); /*Return an iterator at the given index*/\
    ch_array_##TYPE##_iter_t*(*start)(ch_array_##TYPE##_t* this); /*Return an iterator at the start*/\
    ch_array_##TYPE##_iter_t*(*end)(ch_array_##TYPE##_t* this); /*Return an iterator at the end*/\
\
    ch_array_##TYPE##_iter_t*(*find)(ch_array_##TYPE##_t* this, ch_array_##TYPE##_iter_t* begin, ch_array_##TYPE##_iter_t end, TYPE value); /*find the given value using the comparitor function*/\
    void(*sort)(ch_array_##TYPE##_t* this); /*sort into order given the comparitor function*/\
    void(*sort_reverse)(ch_array_##TYPE##_t* this); /*sort into reverse order given the comparitor function*/\
\
    ch_array_##TYPE##_iter_t*(*push_front)(ch_array_##TYPE##_t* this, TYPE value); /* Put an element at the front of the array values, [WARN: In general this is very expensive for an array] */\
    ch_array_##TYPE##_iter_t*(*push_back)(ch_array_##TYPE##_t* this, TYPE value); /* Put an elemenet at the back of the arary values*/\
    ch_array_##TYPE##_iter_t*(*insert_after)(ch_array_##TYPE##_t* this, ch_array_##TYPE##_iter_t* iter, TYPE value); /* Insert an element after the element given by iter*/\
    ch_array_##TYPE##_iter_t*(*insert_before)(ch_array_##TYPE##_t* this, ch_array_##TYPE##_iter_t* iter, TYPE value); /* Insert an element befer the element giver by iter [WARN: In general this is very expensive for an array] */\
    ch_array_##TYPE##_iter_t*(*assign)(ch_array_##TYPE##_t* this, ch_array_##TYPE##_iter_t* iter, TYPE value); /*Set the given iter to the given value */\
    ch_array_##TYPE##_iter_t*(*remove)(ch_array_##TYPE##_t* this, ch_array_##TYPE##_iter_t* iter); /*Remove the given iter [WARN: In general this is very expensive] */\
\
    void(*delete)(ch_array_##TYPE##_t* this); /*Free the resources associated with this array, assumes that individual items have been freed*/ \
\
     /* Members prefixed with "_" are nominally "private" */\
    ch_word (*_cmp)(TYPE lhs, TYPE rhs); /* Comparator function for find and sort operations */\
    TYPE* _array_backing; /*Actual array storage*/\
    ch_word _array_backing_size; /*Number of elements allocated in the given array*/\
    ch_word _array_backing_count; /*NUmber of elements currently in the array*/\
\
};\
\
\
ch_array_##TYPE##_t* ch_array_##TYPE##_new(ch_word size, TYPE init_value, ch_word(*cmp)(TYPE lhs, TYPE rhs) )\



#endif /* ARRAY_H_ */
