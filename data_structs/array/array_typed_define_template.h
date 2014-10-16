/*
 * array_typed.c
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */

#ifndef ARRAY_TYPED_TEMPLATE_DEFINE_H_
#define ARRAY_TYPED_TEMPLATE_DEFINE_H_


#include "array.h"

#include <stdlib.h>
#include <stdio.h>

#define define_array(NAME,TYPE)\
\
static void _update_##NAME(ch_array_##NAME##_t* this)\
{\
    this->size  = this->_array->size;\
    this->first = this->_array->first;\
    this->last  = this->_array->last;\
    this->end   = this->_array->end;\
}\
\
static void _resize_##NAME(ch_array_##NAME##_t* this, ch_word new_size)                 { array_resize(this->_array, new_size); _update_##NAME(this); }\
static ch_word _eq_##NAME(ch_array_##NAME##_t* this, ch_array_##NAME##_t* that)              { ch_word result = array_eq(this->_array, that->_array); _update_##NAME(this); return result;}\
static TYPE* _off_##NAME(ch_array_##NAME##_t* this, ch_word idx)                         { TYPE* result = (TYPE*)array_off(this->_array, idx); _update_##NAME(this); return result; }\
static TYPE* _forward_##NAME(ch_array_##NAME##_t* this, TYPE* ptr, ch_word amount)        { TYPE* result = (TYPE*)array_forward(this->_array, (void*)ptr, amount); _update_##NAME(this); return result; }\
static TYPE* _back_##NAME(ch_array_##NAME##_t* this, TYPE* ptr, ch_word amount)           { TYPE* result =  (TYPE*)array_back(this->_array, (void*)ptr, amount); _update_##NAME(this); return result; }\
static TYPE* _next_##NAME(ch_array_##NAME##_t* this, TYPE* ptr)                           { TYPE* result = (TYPE*)_forward_##NAME(this, ptr, 1); _update_##NAME(this); return result; }\
static TYPE* _prev_##NAME(ch_array_##NAME##_t* this, TYPE* ptr)                           { TYPE* result = (TYPE*)_back_##NAME(this, ptr, 1); _update_##NAME(this); return result; }\
static TYPE* _find_##NAME(ch_array_##NAME##_t* this, TYPE* begin, TYPE* end, TYPE value)    { TYPE* result = (TYPE*) array_find(this->_array, (void*)begin, (void*)end, &value); _update_##NAME(this); return result; }\
static void _sort_##NAME(ch_array_##NAME##_t* this)                                     { array_sort(this->_array); _update_##NAME(this); }\
static TYPE* _from_carray_##NAME(ch_array_##NAME##_t* this, TYPE* carray, ch_word count)  { TYPE* result =  array_from_carray(this->_array, (void*)carray, count); _update_##NAME(this); return result; }\
\
static void _delete_##NAME(ch_array_##NAME##_t* this)\
{\
    if(this->_array){\
        array_delete(this->_array);\
    }\
\
    free(this);\
}\
\
\
ch_array_##NAME##_t* ch_array_##NAME##_new(ch_word size, ch_word (*cmp)(TYPE* lhs, TYPE* rhs) )\
{\
\
    ch_array_##NAME##_t* result = (ch_array_##NAME##_t*)calloc(1,sizeof(ch_array_##NAME##_t));\
    if(!result){\
        printf("Could not allocate memory for new array structure. Giving upn");\
        return ((void *)0);\
    }\
\
    result->_array = ch_array_new(size, sizeof(TYPE), (cmp_void_f)cmp );\
\
\
    /*We have memory to play with, now do all the other assignments*/\
    result->resize                  = _resize_##NAME;\
    result->off                     = _off_##NAME;\
    result->next                    = _next_##NAME;\
    result->prev                    = _prev_##NAME;\
    result->forward                 = _forward_##NAME;\
    result->back                    = _back_##NAME;\
    result->from_carray             = _from_carray_##NAME;\
    result->delete                  = _delete_##NAME;\
\
    /*Fail hard and early if the compare function is NULL*/\
    if(cmp){\
        result->eq                      = _eq_##NAME;\
        result->find                    = _find_##NAME;\
        result->sort                    = _sort_##NAME;\
    }\
\
    _update_##NAME(result);\
\
    return result;\
}

//Regular comparison function
#define define_ch_array_cmp(NAME, TYPE) \
ch_word ch_array_cmp_##NAME(TYPE* lhs, TYPE* rhs)\
{ \
    return ( *lhs == *rhs ? 0 : *lhs < *rhs ? -1 : 1); \
}

//Pointer comparison function
#define define_ch_array_cmpp(NAME, TYPE) ch_word ch_array_cmp_##NAME##p(TYPE* lhs, TYPE* rhs)\
{ \
    return ch_array_cmp_##NAME(*lhs, *rhs);\
}

#define define_ch_array_compare(NAME, TYPE) ch_word ch_array_cmp_##NAME(TYPE* lhs, TYPE* rhs)


#endif //ARRAY_TYPED_TEMPLATE_DEFINE_H_
