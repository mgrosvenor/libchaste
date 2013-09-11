#include "vector_std.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../types/types.h"
#include "../../utils/util.h"

#define _LAST(a) (a + this->_array_backing_count -1)

#define define_ch_vector(TYPE) \
\
static void _resize_##TYPE(ch_vector_##TYPE##_t* this, ch_word new_size)\
{\
    this->_array_backing->resize(this->_array_backing,new_size);\
\
    this->_array_backing_count = MIN(new_size, this->_array_backing_count);\
    this->count                = this->_array_backing_count;\
    this->size                 = new_size;\
\
    if(new_size == 0){\
        this->first          = NULL;\
        this->last           = NULL;\
        this->end            = NULL;\
    }\
    else{\
        this->first            = this->_array_backing->first;\
        this->end              = this->first + this->_array_backing_count;\
        this->last             = this->end - 1;\
    }\
\
}\
\
\
/*Check for equality between two vector lists*/\
static ch_word _eq_##TYPE(ch_vector_##TYPE##_t* this, ch_vector_##TYPE##_t* that)\
{\
\
    if(this->_array_backing_count != that->_array_backing_count){\
        return 0;\
    }\
\
    TYPE* i = this->first;\
    TYPE* j = that->first;\
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
\
/*Return the element at a given offset, with bounds checking*/\
static TYPE* _off_##TYPE(ch_vector_##TYPE##_t* this, ch_word idx)\
{\
\
    if(idx >= this->_array_backing_count){\
        printf("Index (%li) is out of the valid range [%li,%li]", idx, -1 * this->_array_backing_count, this->_array_backing_count - 1 );\
        return NULL;\
    }\
\
    return this->_array_backing->off(this->_array_backing,idx);\
\
}\
\
\
static TYPE* _forward_##TYPE(ch_vector_##TYPE##_t* this, TYPE* ptr, ch_word amount)\
{\
    if(ptr + amount <= this->end){\
        return ptr += amount;\
    }\
\
    return ptr;\
}\
\
static TYPE* _back_##TYPE(ch_vector_##TYPE##_t* this, TYPE* ptr, ch_word amount)\
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
static TYPE* _next_##TYPE(ch_vector_##TYPE##_t* this, TYPE* ptr)\
{\
    return _forward_##TYPE(this, ptr, 1);\
}\
\
static TYPE* _prev_##TYPE(ch_vector_##TYPE##_t* this, TYPE* ptr)\
{\
    return _back_##TYPE(this, ptr, 1);\
}\
\
\
\
/*find the given value using the comparitor function*/\
static TYPE* _find_##TYPE(ch_vector_##TYPE##_t* this, TYPE* begin, TYPE* end, TYPE value)\
{\
    return this->_array_backing->find(this->_array_backing, begin, end, value);\
}\
\
\
\
/* Merge two sorted vectors into out. */\
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
static void _sort_dir_##TYPE(ch_vector_##TYPE##_t* this, ch_word dir)\
{\
    if(this->_array_backing_count <= 1){\
        return; /*Nothing to do here. */\
    }\
\
    /* Grab some temporary auxilary storage */\
    TYPE* aux1 = (TYPE*)malloc(this->_array_backing_count * sizeof(TYPE));\
    if(!aux1){\
        printf("Could not allocate memory for new vector structure. Giving up\n");\
        return;\
    }\
    TYPE* dst = aux1;\
    TYPE* src = this->_array_backing->first;\
\
    ch_word chunk_size = 1;\
    while(chunk_size <= this->_array_backing_count){\
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
            dst = this->_array_backing->first;\
        }\
        else{\
            src = this->_array_backing->first;\
            dst = aux1;\
        }\
    }\
\
\
    if(dst == this->_array_backing->first){\
        memcpy(dst, src, this->_array_backing_count * sizeof(TYPE));\
    }\
\
    free(aux1);\
\
}\
\
\
/*sort into reverse order given the comparitor function*/\
static void _sort_##TYPE(ch_vector_##TYPE##_t* this)\
{\
    _sort_dir_##TYPE(this,1);\
}\
\
\
/*sort into reverse order given the comparitor function*/\
static void _sort_reverse_##TYPE(ch_vector_##TYPE##_t* this)\
{\
    _sort_dir_##TYPE(this,-1);\
}\
\
\
/* Insert an element before the element giver by ptr [WARN: In general this is very expensive for an vector] */\
static TYPE* _insert_before_##TYPE(ch_vector_##TYPE##_t* this, TYPE* ptr, TYPE value)\
{\
\
    /*If the backing memory is full, grow the vector*/\
    if(unlikely(this->_array_backing_count == this->_array_backing->size)){\
        const ch_word ptr_idx = ptr ? ptr - this->_array_backing->first: 0;\
        const ch_word new_size = this->_array_backing->size ? this->_array_backing->size * 2 : 1;\
        _resize_##TYPE(this,new_size);\
        ptr = this->_array_backing->first + ptr_idx;\
    }\
\
    if(unlikely(ptr < this->first)){\
        printf("ptr supplied is out of range. Too small.\n");\
        return NULL;\
    }\
\
    /* NB: It's ok to insert *before* last + 1. This essentially inserts at last which is the last item. */\
    if(unlikely(ptr > this->last + 1)){\
        printf("ptr supplied is out of range. Too big.\n");\
        return NULL;\
    }\
\
    /* Optimise for the push_back case*/\
    if(unlikely(this->_array_backing_count  && ptr <= this->_array_backing->first + this->_array_backing_count )){\
        memmove(ptr + 1,ptr , (this->_array_backing->first + this->_array_backing_count - ptr) * sizeof(TYPE));\
    }\
\
    *ptr = value;\
\
    if(unlikely(this->_array_backing_count == 0)){\
        this->first = this->_array_backing->first;\
        this->last  = this->first;\
        this->end   = this->last + 1;\
    }\
    else{\
        this->last++;\
        this->end++;\
    }\
\
    this->_array_backing_count++;\
    this->count++;\
\
    return ptr;\
}\
\
/* Insert an element after the element given by ptr* [WARN: In general this is very expensive for an vector] */\
static TYPE* _insert_after_##TYPE(ch_vector_##TYPE##_t* this, TYPE* ptr, TYPE value)\
{\
    /*Inserting after is the equivalent to inserting before, the value after the current */\
    ptr = this->_array_backing->size == 0 ? NULL : ptr + 1;\
    return _insert_before_##TYPE(this,ptr,value);\
}\
\
\
\
/* Put an element at the front of the vector values, [WARN: In general this is very expensive for an vector] */\
static TYPE* _push_front_##TYPE(ch_vector_##TYPE##_t* this, TYPE value)\
{\
    /* Pushing onto the front is equivalent to inserting at the head */\
    TYPE* ptr = this->_array_backing->size == 0 ? NULL : this->first;\
    return _insert_before_##TYPE(this, ptr, value);\
}\
\
/* Put an element at the back of the arary values*/\
static TYPE* _push_back_##TYPE(ch_vector_##TYPE##_t* this, TYPE value)\
{\
    /* Pushing onto the end is equivalent to inserting at the tail */\
    TYPE* ptr = this->_array_backing->size == 0 ? NULL : this->end;\
    return _insert_before_##TYPE(this, ptr, value);\
}\
\
\
/*Remove the given ptr [WARN: In general this is very expensive], return an to the next item in the list */\
static TYPE* _remove_##TYPE(ch_vector_##TYPE##_t* this, TYPE* ptr)\
{\
\
    if(unlikely(this->first == this->end)){\
        printf("Array list is emptyn");\
        return NULL;\
    }\
\
    if(unlikely(ptr < this->_array_backing->first)){\
        printf("ptr supplied is out of range. Too small.\n");\
        return NULL;\
    }\
\
    if(unlikely(ptr > this->last)){\
        printf("ptr supplied is out of range. Too big.\n");\
        return NULL;\
    }\
\
    /*Slow (but likely), keep the fast path fast*/\
    if(unlikely(ptr != this->last)){\
/*\
        printf("ptr:%p[%li], first:%p[%li], last:%p[%li], end:%p:[%li] (%lu)\n",\
                (void*)ptr, ptr - this->first,\
                (void*)this->first, this->first - this->first,\
                (void*)this->last, this->last - this->first,\
                (void*)this->end, this->end - this->first,\
                (this->last) - (ptr));\
*/\
        memmove(ptr, ptr + 1, (this->last) - (ptr) );\
    }\
\
    this->_array_backing_count--;\
    this->count--;\
\
\
    if(unlikely(this->_array_backing_count == 0)){\
        this->first = this->_array_backing->first;\
        this->last = this->first;\
        this->end  = this->last;\
        ptr = NULL;\
    }\
    else{\
        this->last--;\
        this->end--;\
    }\
\
    return ptr;\
}\
\
/* Put an element at the front of the vector values, [WARN: In general this is very expensive for an vector] */\
void _pop_front_##TYPE(ch_vector_##TYPE##_t* this)\
{\
    _remove_##TYPE(this,this->first);\
}\
\
/* Put an element at the front of the vector values, [WARN: In general this is very expensive for an vector] */\
void _pop_back_##TYPE(ch_vector_##TYPE##_t* this)\
{\
    _remove_##TYPE(this,this->last);\
}\
\
static void _delete_##TYPE(ch_vector_##TYPE##_t* this)\
{\
    if(this->_array_backing){\
        this->_array_backing->delete(this->_array_backing);\
    }\
\
    free(this);\
}\
\
\
/*Assign at most size elements from the C vector*/\
static TYPE* _push_back_carray_##TYPE(ch_vector_##TYPE##_t* this, TYPE* cvector, ch_word count)\
{\
    if(this->_array_backing->size - this->_array_backing_count < count){\
        const ch_word new_size = this->_array_backing->size ? this->_array_backing->size * 2 : next_pow2(count);\
        _resize_##TYPE(this,new_size);\
    }\
\
    memcpy(this->end, cvector, count * sizeof(TYPE) );\
    this->_array_backing_count += count;\
    this->count = this->_array_backing_count;\
    this->last += count;\
    this->end = this->last + 1;\
\
    return this->last;\
}\
\
\
ch_vector_##TYPE##_t* ch_vector_##TYPE##_new(ch_word size, ch_word (*cmp)(TYPE lhs, TYPE rhs) )\
{\
\
    ch_vector_##TYPE##_t* result = (ch_vector_##TYPE##_t*)calloc(1,sizeof(ch_vector_##TYPE##_t));\
    if(!result){\
        printf("Could not allocate memory for new vector structure. Giving up\n");\
        return NULL;\
    }\
\
    result->_array_backing       = ch_array_##TYPE##_new(size, cmp);\
\
    /*We have memory to play with, now do all the other assignments*/\
    result->_array_backing_count    = 0;\
    result->_cmp                    = cmp;\
    result->first                   = result->_array_backing->first;\
    result->last                    = result->first;\
    result->end                     = result->first;\
    result->size                    = result->_array_backing->size;\
    result->count                   = result->_array_backing_count;\
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
    result->push_front              = _push_front_##TYPE;\
    result->pop_front               = _pop_front_##TYPE;\
    result->push_back               = _push_back_##TYPE;\
    result->pop_front               = _pop_front_##TYPE;\
    result->insert_after            = _insert_after_##TYPE;\
    result->insert_before           = _insert_before_##TYPE;\
    result->remove                  = _remove_##TYPE;\
    result->push_back_carray        = _push_back_carray_##TYPE;\
    result->delete                  = _delete_##TYPE;\
\
    return result;\
} 
