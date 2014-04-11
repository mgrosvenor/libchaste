/*
 * array_typed.h
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */

#ifndef ARRAY_TYPED_TEMPLATE_DECLARE_H_
#define ARRAY_TYPED_TEMPLATE_DECLARE_H_


#include "../../types/types.h"
#include "array.h"

#define declare_array(NAME,TYPE)\
\
struct ch_array_##NAME;\
typedef struct ch_array_##NAME  ch_array_##NAME##_t;\
\
struct ch_array_##NAME{\
    ch_word size;  /*Return the max number number of elements in the array list*/\
    TYPE* first; /*Pointer to the fist valid entry list. Not valid if first == end*/\
    TYPE* last; /*Pointer to the last valid element in the array. Not valid if last == end*/\
    TYPE* end; /*Pointer to the one element beyond the end of the valid elements in array. Do not dereference! */\
\
    void (*resize)(ch_array_##NAME##_t* this, ch_word new_size); /*Resize the array*/\
    ch_word (*eq)(ch_array_##NAME##_t* this, ch_array_##NAME##_t* that); /*Check for equality*/\
\
    TYPE* (*off)(ch_array_##NAME##_t* this, ch_word idx); /*Return the element at a given offset, with bounds checking*/\
    TYPE* (*next)(ch_array_##NAME##_t* this, TYPE* ptr);  /*Step forwards by one entry*/\
    TYPE* (*prev)(ch_array_##NAME##_t* this, TYPE* ptr); /*Step backwards by one entry*/\
    TYPE* (*forward)(ch_array_##NAME##_t* this, TYPE* ptr, ch_word amount);  /*Step forwards by amount*/\
    TYPE* (*back)(ch_array_##NAME##_t* this, TYPE* ptr, ch_word amount);  /*Step backwards by amount*/\
\
    TYPE* (*find)(ch_array_##NAME##_t* this, TYPE* begin, TYPE* end, TYPE value); /*find the given value using the comparator function*/\
    void (*sort)(ch_array_##NAME##_t* this); /*sort into order given the comparator function*/\
\
    void (*delete)(ch_array_##NAME##_t* this); /*Free the resources associated with this array, assumes that individual items have been freed*/\
\
    TYPE* (*from_carray)(ch_array_##NAME##_t* this, TYPE* carray, ch_word count); /*Set at most count elements to the value in carray starting at offset in this array*/\
\
    ch_array_t* _array;\
};\
\
ch_array_##NAME##_t* ch_array_##NAME##_new(ch_word size, ch_word (*cmp)(TYPE* lhs, TYPE* rhs) );\


#define CH_ARRAY(NAME)  ch_array_##NAME##_t
#define CH_ARRAY_NEW(NAME, size, cmp) ch_array_##NAME##_new(size, cmp )

#endif //ARRAY_TYPED_TEMPLATE_DECLARE_H_
