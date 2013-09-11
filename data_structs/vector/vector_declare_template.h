// CamIO 2: vector.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef VECTOR_DECLARE_TEMPLATE_H_
#define VECTOR_DECLARE_TEMPLATE_H_

#include "../../types/types.h"


#define declare_ch_vector(TYPE) \
\
struct ch_vector_##TYPE;\
typedef struct ch_vector_##TYPE ch_vector_##TYPE##_t;\
\
\
struct ch_vector_##TYPE{\
    ch_word size;  /*Return the max number number of elements in the vector list*/\
    ch_word count;  /*Return the actual number of elements in the vector*/\
    void (*resize)(ch_vector_##TYPE##_t* this, ch_word new_size); /*Resize the vector*/\
    ch_word (*eq)(ch_vector_##TYPE##_t* this, ch_vector_##TYPE##_t* that); /*Check for equality*/\
\
    TYPE* (*off)(ch_vector_##TYPE##_t* this, ch_word idx); /*Return the element at a given offset, with bounds checking*/\
    TYPE* first; /*Pointer to the fist valid entry list. Not valid if first == end*/\
    TYPE* last; /*Pointer to the last valid element in the list. Not valid if last == end*/\
    TYPE* end; /*Pointer to the one element beyond the end of the valid elements in list. Do not dereference! */\
\
    TYPE* (*next)(ch_vector_##TYPE##_t* this, TYPE* ptr);  /*Step forwards by one entry*/ \
    TYPE* (*prev)(ch_vector_##TYPE##_t* this, TYPE* ptr); /*Step backwards by one entry*/\
    TYPE* (*forward)(ch_vector_##TYPE##_t* this, TYPE* ptr, ch_word amount);  /*Step forwards by amount*/ \
    TYPE* (*back)(ch_vector_##TYPE##_t* this, TYPE* ptr, ch_word amount);  /*Step backwards by amount*/ \
\
    TYPE* (*find)(ch_vector_##TYPE##_t* this, TYPE* begin, TYPE* end, TYPE value); /*find the given value using the comparator function*/\
    void (*sort)(ch_vector_##TYPE##_t* this); /*sort into order given the comparator function*/\
    void (*sort_reverse)(ch_vector_##TYPE##_t* this); /*sort into reverse order given the comparator function*/\
\
    TYPE* (*push_front)(ch_vector_##TYPE##_t* this, TYPE value); /* Put an element at the front of the vector list values, [WARN: In general this is very expensive for an vector] */\
    void (*pop_front)(ch_vector_##TYPE##_t* this); /* Push an element off the front of the vector list values, [WARN: In general this is very expensive for an vector] */\
    TYPE* (*push_back)(ch_vector_##TYPE##_t* this, TYPE value); /* Put an element at the back of the vector values*/\
    void (*pop_back)(ch_vector_##TYPE##_t* this); /* Push an element at the back of the vector values*/\
\
    TYPE* (*insert_after)(ch_vector_##TYPE##_t* this, TYPE* ptr, TYPE value); /* Insert an element after the element given by ptr*/\
    TYPE* (*insert_before)(ch_vector_##TYPE##_t* this, TYPE* ptr, TYPE value); /* Insert an element before the element giver by ptr [WARN: In general this is very expensive for an vector] */\
    TYPE* (*remove)(ch_vector_##TYPE##_t* this, TYPE* ptr); /*Remove the given ptr [WARN: In general this is very expensive] */\
\
    void (*delete)(ch_vector_##TYPE##_t* this); /*Free the resources associated with this vector, assumes that individual items have been freed*/ \
\
    TYPE* (*push_back_carray)(ch_vector_##TYPE##_t* this, TYPE* cvector, ch_word count); /*Push back count elements the C vector to the back vector-list*/\
\
     /* Members prefixed with "_" are nominally "private" Don't touch my privates!*/\
    ch_word (*_cmp)(TYPE lhs, TYPE rhs); /* Comparator function for find and sort operations */\
    TYPE* _array_backing; /*Actual vector storage*/\
    ch_word _array_backing_size; /*Number of elements allocated in the given vector*/\
    ch_word _array_backing_count; /*NUmber of elements currently in the vector*/\
\
};\
\
\
ch_vector_##TYPE##_t* ch_vector_##TYPE##_new(ch_word size, ch_word(*cmp)(TYPE lhs, TYPE rhs) )\



#endif /* VECTOR_DECLARE_H_ */
