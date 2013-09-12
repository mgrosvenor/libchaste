/*
 * array_typed.c
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */


#include "array.h"

#include <stdlib.h>
#include <stdio.h>


#define define_array(TYPE)\
\
static void _update_##TYPE(ch_array_##TYPE##_t* this)\
{\
    this->size  = this->_array->size;\
    this->first = this->_array->first;\
    this->last  = this->_array->last;\
    this->end   = this->_array->end;\
}\
\
static void _resize_##TYPE(ch_array_##TYPE##_t* this, ch_word new_size)                 { array_resize(this->_array, new_size); _update_##TYPE(this); }\
static ch_word _eq_##TYPE(ch_array_##TYPE##_t* this, ch_array_##TYPE##_t* that)              { ch_word result = array_eq(this->_array, that->_array); _update_##TYPE(this); return result;}\
static TYPE* _off_##TYPE(ch_array_##TYPE##_t* this, ch_word idx)                         { TYPE* result = (TYPE*)array_off(this->_array, idx); _update_##TYPE(this); return result; }\
static TYPE* _forward_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr, ch_word amount)        { TYPE* result = (TYPE*)array_forward(this->_array, (void*)ptr, amount); _update_##TYPE(this); return result; }\
static TYPE* _back_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr, ch_word amount)           { TYPE* result =  (TYPE*)array_back(this->_array, (void*)ptr, amount); _update_##TYPE(this); return result; }\
static TYPE* _next_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr)                           { TYPE* result = (TYPE*)_forward_##TYPE(this, ptr, 1); _update_##TYPE(this); return result; }\
static TYPE* _prev_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr)                           { TYPE* result = (TYPE*)_back_##TYPE(this, ptr, 1); _update_##TYPE(this); return result; }\
static TYPE* _find_##TYPE(ch_array_##TYPE##_t* this, TYPE* begin, TYPE* end, TYPE value)    { TYPE* result = (TYPE*) array_find(this->_array, (void*)begin, (void*)end, &value); _update_##TYPE(this); return result; }\
static void _sort_##TYPE(ch_array_##TYPE##_t* this)                                     { array_sort(this->_array); _update_##TYPE(this); }\
static TYPE* _from_carray_##TYPE(ch_array_##TYPE##_t* this, TYPE* carray, ch_word count)  { TYPE* result =  array_from_carray(this->_array, (void*)carray, count); _update_##TYPE(this); return result; }\
\
static void _delete_##TYPE(ch_array_##TYPE##_t* this)\
{\
    if(this->_array){\
        array_delete(this->_array);\
    }\
\
    free(this);\
}\
\
\
ch_array_##TYPE##_t* ch_array_##TYPE##_new(ch_word size, ch_word (*cmp)(TYPE* lhs, TYPE* rhs) )\
{\
\
    ch_array_##TYPE##_t* result = (ch_array_##TYPE##_t*)calloc(1,sizeof(ch_array_##TYPE##_t));\
    if(!result){\
        printf("Could not allocate memory for new array structure. Giving upn");\
        return ((void *)0);\
    }\
\
    result->_array = ch_array_new(size, sizeof(TYPE), (cmp_void_f)cmp );\
\
\
    /*We have memory to play with, now do all the other assignments*/\
    result->resize                  = _resize_##TYPE;\
    result->off                     = _off_##TYPE;\
    result->next                    = _next_##TYPE;\
    result->prev                    = _prev_##TYPE;\
    result->forward                 = _forward_##TYPE;\
    result->back                    = _back_##TYPE;\
    result->from_carray             = _from_carray_##TYPE;\
    result->delete                  = _delete_##TYPE;\
\
    /*Fail hard and early if the compare function is NULL*/\
    if(cmp){\
        result->eq                      = _eq_##TYPE;\
        result->find                    = _find_##TYPE;\
        result->sort                    = _sort_##TYPE;\
    }\
\
    _update_##TYPE(result);\
\
    return result;\
}
