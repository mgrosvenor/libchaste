// CamIO 2: array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk)
// Licensed under BSD 3 Clause, please see LICENSE for more details.

#ifndef ARRAY_DEFINE_TEMPLATE_H_
#define ARRAY_DEFINE_TEMPLATE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "array_std.h"
//#include "../log/log.h"

#include "../../types/types.h"
#include "../../utils/util.h"

#define _LAST(a) (a + this->_array_backing_size -1)

#define define_ch_array(TYPE) \
\
static void _resize_##TYPE(ch_array_##TYPE##_t* this, ch_word new_size)\
{\
    this->_array_backing = (TYPE*)realloc(this->_array_backing, new_size * sizeof(TYPE));\
    if(!this->_array_backing){\
        printf("Could not allocate memory for backing store\n");\
        return;\
    }\
\
    this->_array_backing_size  = new_size;\
    this->size                 = new_size;\
\
    if(new_size == 0){\
        this->first          = this->_array_backing;\
        this->last           = this->_array_backing;\
        this->end            = this->_array_backing;\
    }\
    else{\
        this->first            = this->_array_backing;\
        this->last             = _LAST(this->_array_backing);\
        this->end              = this->last + 1;\
    }\
\
}\
\
\
/*Check for equality between two array lists*/\
static ch_word _eq_##TYPE(ch_array_##TYPE##_t* this, ch_array_##TYPE##_t* that)\
{\
\
    if(this->_array_backing_size != that->_array_backing_size){\
        return 0; \
    }\
\
    TYPE* i = this->first; \
    TYPE* j = that->first; \
\
    for(; i < this->end && j < that->end; i = this->next(this, i), j = that->next(that, j)){\
        if( this->_cmp(*i,*j) ){\
            return 0;\
        }\
    }\
\
    return 1;\
}\
\
\
/*Take an index an fix it so that negative indexs are legal */\
static inline ch_word range_fix_##TYPE(ch_array_##TYPE##_t* this, ch_word idx)\
{\
    idx = idx < 0 ? idx + this->_array_backing_size : idx;\
\
    if(idx >= 0 && idx < this->_array_backing_size){\
        return idx;\
    }\
\
    printf("Index (%li) is out of the valid range [%li,%li]\n", idx, -1 * this->_array_backing_size, this->_array_backing_size - 1 );\
    return -1;\
\
}\
\
\
/*Return the element at a given offset, with bounds checking*/\
static TYPE* _off_##TYPE(ch_array_##TYPE##_t* this, ch_word idx)\
{\
\
    if(this->_array_backing_size < 1){\
        printf("Index (%li) is out of the valid range [%li,%li]\n", idx, -1 * this->_array_backing_size, this->_array_backing_size - 1 );\
        return NULL;\
    }\
\
    idx = range_fix_##TYPE(this, idx);\
    if(idx >= 0){\
        return &this->_array_backing[idx];\
    }\
\
    return NULL;\
\
}\
\
\
static TYPE* _forward_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr, ch_word amount)\
{\
    if(ptr + amount <= this->end){\
        return ptr += amount;\
    }\
\
    return ptr;\
}\
\
static TYPE* _back_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr, ch_word amount)\
{\
    if(ptr - amount >= this->first){\
        return ptr -= amount;\
    }\
\
    return ptr;\
}\
\
\
\
static TYPE* _next_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr)\
{\
    return _forward_##TYPE(this, ptr, 1);\
}\
\
static TYPE* _prev_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr)\
{\
    return _back_##TYPE(this, ptr, 1);\
}\
\
\
\
/*find the given value using the comparator function*/\
static TYPE* _find_##TYPE(ch_array_##TYPE##_t* this, TYPE* begin, TYPE* end, TYPE value)\
{\
    if(unlikely(begin < this->first || begin > this->end)){\
        printf("Begin iterator supplied is out of range is not a member of this array\n");\
        return NULL;\
    }\
\
    if(unlikely(end < this->first || end > this->end)){\
        printf("End iterator supplied is out of range is not a member of this array\n");\
        return NULL;\
    }\
\
    for(TYPE* it = begin; it != end; it++){\
        if(this->_cmp(*it,value) == 0 ){\
            return it;\
        }\
    }\
\
    return NULL;\
}\
\
\
\
/* Merge two sorted arrays into out. */\
static inline TYPE* _merge_##TYPE(TYPE* out, TYPE* lhs_lo, TYPE* lhs_hi, TYPE* rhs_lo, TYPE* rhs_hi, ch_word (*cmp)(TYPE lhs, TYPE rhs), ch_word dir)\
{\
\
    TYPE* lhs_ptr = lhs_lo;\
    TYPE* rhs_ptr = rhs_lo;\
\
    while(1){\
\
        while(lhs_ptr <= lhs_hi &&  dir * cmp(*lhs_ptr,*rhs_ptr) <= 0){\
            lhs_ptr++;\
        }\
\
        const ch_word lhs_cpy_count = lhs_ptr - lhs_lo;\
        const ch_word lhs_cpy_bytes  = lhs_cpy_count * sizeof(TYPE);\
        memcpy(out,lhs_lo,lhs_cpy_bytes);\
        lhs_lo = lhs_ptr;\
        out += lhs_cpy_count;\
\
        if(lhs_lo > lhs_hi){\
            break;\
        }\
\
        while(rhs_ptr <= rhs_hi && dir * cmp(*rhs_ptr,*lhs_ptr) <= 0){\
            rhs_ptr++;\
        }\
\
        const ch_word rhs_cpy_count = rhs_ptr - rhs_lo;\
        const ch_word rhs_cpy_bytes  = rhs_cpy_count * sizeof(TYPE);\
        memcpy(out,rhs_lo,rhs_cpy_bytes);\
        rhs_lo = rhs_ptr;\
        out += rhs_cpy_count;\
\
        if(rhs_lo > rhs_hi){\
            break;\
        }\
    }\
\
\
\
    if(lhs_lo <= lhs_hi){\
        const ch_word lhs_cpy_count = lhs_hi - lhs_lo + 1;\
        const ch_word lhs_cpy_bytes  = lhs_cpy_count * sizeof(TYPE);\
        memcpy(out,lhs_lo,lhs_cpy_bytes);\
        out += lhs_cpy_count;\
    }\
\
    if(rhs_lo <= rhs_hi){\
        const ch_word rhs_cpy_count = rhs_hi - rhs_lo + 1;\
        const ch_word rhs_cpy_bytes  = rhs_cpy_count * sizeof(TYPE);\
        memcpy(out,rhs_lo,rhs_cpy_bytes);\
        out += rhs_cpy_count;\
    }\
\
    return out;\
\
}\
\
\
\
/*sort into order given the comparitor function dir -1 = down, 1 = up*/\
static void _sort_dir_##TYPE(ch_array_##TYPE##_t* this, ch_word dir)\
{\
    if(this->_array_backing_size <= 1){\
        return; /*Nothing to do here. */\
    }\
\
    /* Grab some temporary auxilary storage */\
    TYPE* aux1 = (TYPE*)malloc(this->_array_backing_size * sizeof(TYPE));\
    if(!aux1){\
        printf("Could not allocate memory for new array structure. Giving up\n");\
        return;\
    }\
    TYPE* dst = aux1;\
    TYPE* src = this->_array_backing;\
\
    ch_word chunk_size = 1;\
    while(chunk_size <= this->_array_backing_size){\
        TYPE* dst_ptr = dst;\
        TYPE* lhs_lo  = src;\
        TYPE* lhs_hi  = lhs_lo + chunk_size - 1;\
        TYPE* rhs_lo  = lhs_hi + 1;\
        TYPE* rhs_hi  = rhs_lo + chunk_size - 1;\
\
        while(1){\
            if(lhs_lo > _LAST(src)){\
                break;\
            }\
\
            if(rhs_lo <= _LAST(src) && rhs_hi > _LAST(src)){\
                rhs_hi = _LAST(src);\
            }\
\
            if(lhs_lo <= _LAST(src) && lhs_hi > _LAST(src)){\
                lhs_hi = _LAST(src);\
            }\
\
\
            /* There is no right hand side, so just copy the lhs bytes */\
            if(rhs_lo > _LAST(src)){\
                const ch_word lhs_cpy_count = lhs_hi - lhs_lo + 1;\
                const ch_word lhs_cpy_bytes  = lhs_cpy_count * sizeof(TYPE);\
                memcpy(dst_ptr,lhs_lo, lhs_cpy_bytes );\
                break;\
            }\
\
            dst_ptr = _merge_##TYPE(dst_ptr,lhs_lo, lhs_hi, rhs_lo, rhs_hi, this->_cmp, dir);\
\
            lhs_lo = rhs_hi + 1;\
            lhs_hi = lhs_lo + chunk_size -1;\
            rhs_lo = lhs_hi + 1;\
            rhs_hi = rhs_lo + chunk_size - 1;\
        }\
\
        chunk_size *= 2;\
        if(dst == aux1){\
            src = aux1;\
            dst = this->_array_backing;\
        }\
        else{\
            src = this->_array_backing;\
            dst = aux1;\
        }\
    }\
\
\
    if(dst == this->_array_backing){\
        memcpy(dst, src, this->_array_backing_size * sizeof(TYPE));\
    }\
\
    free(aux1);\
\
}\
\
\
/*sort into reverse order given the comparitor function*/\
static void _sort_##TYPE(ch_array_##TYPE##_t* this)\
{\
    _sort_dir_##TYPE(this,1);\
}\
\
\
/*sort into reverse order given the comparitor function*/\
static void _sort_reverse_##TYPE(ch_array_##TYPE##_t* this)\
{\
    _sort_dir_##TYPE(this,-1);\
}\
\
\
\
static void _delete_##TYPE(ch_array_##TYPE##_t* this)\
{\
    if(this->_array_backing){\
        free(this->_array_backing);\
    }\
\
    free(this);\
}\
\
\
/*Assign at most size elements from the C array*/\
static TYPE* _from_carray_##TYPE(ch_array_##TYPE##_t* this, TYPE* carray, ch_word count)\
{\
    count = MIN(count,this->_array_backing_size);\
    memcpy(this->first, carray,  count * sizeof(TYPE) );\
\
    return this->last;\
}\
\
\
ch_array_##TYPE##_t* ch_array_##TYPE##_new(ch_word size, ch_word (*cmp)(TYPE lhs, TYPE rhs) )\
{\
\
    ch_array_##TYPE##_t* result = (ch_array_##TYPE##_t*)calloc(1,sizeof(ch_array_##TYPE##_t));\
    if(!result){\
        printf("Could not allocate memory for new array structure. Giving up\n");\
        return NULL;\
    }\
\
    if(size > 0){\
        result->_array_backing       = calloc(size,sizeof(TYPE));\
        if(!result->_array_backing){\
            printf("Could not allocate memory for new array backing. Giving upn");\
            free(result);\
            return NULL;\
        }\
    }\
    else{\
        result->_array_backing = NULL;\
    }\
\
    /*We have memory to play with, now do all the other assignments*/\
    result->_array_backing_size     = size;\
    result->_cmp                    = cmp;\
    result->first                   = result->_array_backing;\
    result->last                    = result->first ? result->_array_backing + size -1 : NULL;\
    result->end                     = result->first ? result->last + 1 : NULL;\
    result->size                    = result->_array_backing_size;\
    result->resize                  = _resize_##TYPE;\
    result->eq                      = _eq_##TYPE;\
    result->off                     = _off_##TYPE;\
    result->next                    = _next_##TYPE;\
    result->prev                    = _prev_##TYPE;\
    result->forward                 = _forward_##TYPE;\
    result->back                    = _back_##TYPE;\
    result->find                    = _find_##TYPE;\
    result->sort                    = _sort_##TYPE;\
    result->sort_reverse            = _sort_reverse_##TYPE;\
    result->from_carray             = _from_carray_##TYPE;\
    result->delete                  = _delete_##TYPE;\
\
    return result;\
}


#endif /* ARRAY_DEFINE_TEMPLATE_H_ */

