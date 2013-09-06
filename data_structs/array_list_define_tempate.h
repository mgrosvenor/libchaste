// CamIO 2: array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk)
// Licensed under BSD 3 Clause, please see LICENSE for more details.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "array_list_std.h"
//#include "../log/log.h"

#include "../types/types.h"
#include "../utils/util.h"

#define define_ch_array(TYPE) \
\
/*Resize the array*/\
static void _resize_##TYPE(ch_array_##TYPE##_t* this, ch_word new_size)\
{\
    this->_array_backing = (TYPE*)realloc(this->_array_backing, new_size * sizeof(TYPE));\
    if(!this->_array_backing){\
        printf("Could not allocate memory for backing store\n");\
        return;\
    }\
\
    if(new_size < this->_array_backing_size){\
        this->end = this->_array_backing + new_size;\
        this->last = this->end - 1;\
        this->_array_backing_count = new_size;\
        this->count = this->_array_backing_count;\
    }\
\
    this->_array_backing_size = new_size;\
    this->size = new_size; /* Update the public size*/\
\
}\
\
/*Take an index an fix it so that negative indexs are legal */\
static inline ch_word range_fix_##TYPE(ch_array_##TYPE##_t* this, ch_word idx)\
{\
    idx = idx < 0 ? idx + this->_array_backing_size : idx;\
\
    if(idx >= 0 && idx < this->_array_backing_count && idx < this->_array_backing_size){\
        return idx;\
    }\
\
    printf("Index (%li) is out of the valid range [%li,%li]", idx, -1 * this->_array_backing_size, this->_array_backing_size - 1 );\
    return -1;\
\
}\
\
\
/*Return the element at a given offset, with bounds checking*/\
static TYPE* _off_##TYPE(ch_array_##TYPE##_t* this, ch_word idx)\
{\
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
/*find the given value using the comparitor function*/\
static TYPE* _find_##TYPE(ch_array_##TYPE##_t* this, TYPE* begin, TYPE* end, TYPE value)\
{\
    for(TYPE* it = begin; it != end; it++){\
        if(this->_cmp(*it,value) == 0 ){\
            return it;\
        }\
    }\
\
    return NULL;\
}\
\
/*sort TYPEo order given the comparitor function*/\
static void _sort_##TYPE(ch_array_##TYPE##_t* this)\
{\
    (void)this;\
}\
\
/*sort TYPEo reverse order given the comparitor function*/\
static void _sort_reverse_##TYPE(ch_array_##TYPE##_t* this)\
{\
    (void)this;\
}\
\
\
/* Insert an element befer the element giver by ptr [WARN: In general this is very expensive for an array] */\
static TYPE* _insert_before_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr, TYPE value)\
{\
\
    if(unlikely(ptr < this->_array_backing)){\
        printf("ptr supplied is out of range. Too small.\n");\
        return NULL;\
    }\
\
    /* NB: It's ok to insert before count + 1. This essentially inserts at count which is the last item. */\
    if(unlikely(ptr > this->_array_backing + this->_array_backing_count + 1)){\
        printf("ptr supplied is out of range. Too big.\n");\
        return NULL;\
    }\
\
    if(unlikely(this->_array_backing_count >= this->_array_backing_size)){\
        printf("Array list is full!\n");\
        return NULL;\
\
    }\
\
    if(likely(this->_array_backing_count  && ptr <= this->_array_backing + this->_array_backing_count )){\
\
\
        printf("ptr:%p[%li], first:%p[%li], last:%p[%li], end:%p:[%li]\n",\
                (void*)ptr, ptr - this->first,\
                (void*)this->first, this->first - this->first,\
                (void*)this->last, this->last - this->first,\
                (void*)this->end, this->end - this->first );\
\
\
        printf("Moving %lu\n", this->_array_backing + this->_array_backing_count - ptr);\
\
        memmove(ptr + 1,ptr , (this->_array_backing + this->_array_backing_count - ptr) * sizeof(TYPE));\
    }\
    else{\
        printf("ptr:%p[%li], first:%p[%li], last:%p[%li], end:%p:[%li]\n",\
                (void*)ptr, ptr - this->first,\
                (void*)this->first, this->first - this->first,\
                (void*)this->last, this->last - this->first,\
                (void*)this->end, this->end - this->first );\
\
        printf("Not moving\n");\
    }\
\
    *ptr = value;\
\
    if(unlikely(this->_array_backing_count == 0)){\
        this->last = this->_array_backing;\
        this->first = this->_array_backing;\
    }\
    else{\
        this->last++;\
    }\
\
    this->_array_backing_count++;\
    this->count++;\
    this->end++;\
\
    return ptr;\
}\
\
/* Insert an element after the element given by ptr* [WARN: In general this is very expensive for an array] */\
static TYPE* _insert_after_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr, TYPE value)\
{\
    /*Inserting after is the equivalent to inserting before, the value after the current */\
    return _insert_before_##TYPE(this,ptr++,value);\
}\
\
\
/* Put an element at the front of the array values, [WARN: In general this is very expensive for an array] */\
static TYPE* _push_front_##TYPE(ch_array_##TYPE##_t* this, TYPE value)\
{\
    /* Pushing onto the front is equivalent to inserting at the head */\
    return _insert_before_##TYPE(this, this->_array_backing, value);\
}\
\
/* Put an element at the back of the arary values*/\
static TYPE* _push_back_##TYPE(ch_array_##TYPE##_t* this, TYPE value)\
{\
    /* Pushing onto the end is equivalent to inserting at the tail */\
    _insert_before_##TYPE(this, this->end, value);\
    return NULL;\
}\
\
\
/*Remove the given ptr [WARN: In general this is very expensive], return an to the next item in the list */\
static TYPE* _remove_##TYPE(ch_array_##TYPE##_t* this, TYPE* ptr)\
{\
\
    if(unlikely(this->first == this->end)){\
        printf("Array list is empty\n");\
        return NULL;\
    }\
\
    if(unlikely(ptr < this->_array_backing)){\
        printf("ptr supplied is out of range. Too small.\n");\
        return NULL;\
    }\
\
    if(unlikely(ptr > this->_array_backing + this->_array_backing_count)){\
        printf("ptr supplied is out of range. Too big.\n");\
        return NULL;\
    }\
\
    /*Slow path*/\
    if(unlikely(ptr != this->last)){\
/*\
        printf("ptr:%p[%li], first:%p[%li], last:%p[%li], end:%p:[%li]",\
                (void*)ptr, ptr - this->first,\
                (void*)this->first, this->last - this->first,\
                (void*)this->last, this->last - this->first,\
                (void*)this->end, this->end - this->first );\
*/\
        memmove(ptr, ptr + 1, (this->last) - (ptr + 1) );\
    }\
\
    this->_array_backing_count--;\
    this->count--;\
\
\
    if(unlikely(this->_array_backing_count == 0)){\
        this->last  = this->_array_backing;\
        this->first = this->_array_backing;\
        this->end   = this->_array_backing;\
    }\
    else{\
        this->last--;\
        this->end--;\
    }\
\
    return ptr;\
}\
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
ch_array_##TYPE##_t* ch_array_##TYPE##_new(ch_word size, ch_word (*cmp)(TYPE lhs, TYPE rhs) )\
{\
\
    ch_array_##TYPE##_t* result = (ch_array_##TYPE##_t*)calloc(1,sizeof(ch_array_##TYPE##_t));\
    if(!result){\
        printf("Could not allocate memory for new array structure. Giving up\n");\
        return NULL;\
    }\
\
    result->_array_backing       = calloc(size,sizeof(TYPE));\
    if(!result->_array_backing){\
        printf("Could not allocate memory for new array backing. Giving up\n");\
        free(result);\
        return NULL;\
    }\
\
    /*We have memory to play with, now do all the other assignments*/\
    result->_array_backing_size  	= size;\
    result->_array_backing_count 	= 0;\
    result->_cmp                 	= cmp;\
    result->first				 	= result->_array_backing;\
    result->last				 	= result->_array_backing;\
    result->end				     	= result->_array_backing;\
    result->size            		= result->_array_backing_size;\
    result->count           		= result->_array_backing_count;\
    result->resize          		= _resize_##TYPE;\
    result->off             		= _off_##TYPE;\
    result->next                    = _next_##TYPE;\
    result->prev                    = _prev_##TYPE;\
    result->forward                 = _forward_##TYPE;\
    result->back                    = _back_##TYPE;\
    result->find            		= _find_##TYPE;\
    result->sort           		 	= _sort_##TYPE;\
    result->sort_reverse    		= _sort_reverse_##TYPE;\
    result->push_front      		= _push_front_##TYPE;\
    result->push_back       		= _push_back_##TYPE;\
    result->insert_after    		= _insert_after_##TYPE;\
    result->insert_before   		= _insert_before_##TYPE;\
    result->remove                  = _remove_##TYPE;\
    result->delete          		= _delete_##TYPE;\
\
    return result;\
}
