#include "vector.h"
#include "vector_std.h"

#include "../../utils/util.h"
#include "../../types/types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define define_ch_vector(NAME,TYPE)\
\
static void _update_##NAME(ch_vector_##NAME##_t* this)\
{\
    this->size  = this->_vector->size;\
    this->first = this->_vector->first;\
    this->last  = this->_vector->last;\
    this->end   = this->_vector->end;\
    this->count = this->_vector->count;\
}\
\
static void _resize_##NAME(ch_vector_##NAME##_t* this, ch_word new_size)                    { vector_resize(this->_vector, new_size); _update_##NAME(this); }\
static ch_word _eq_##NAME(ch_vector_##NAME##_t* this, ch_vector_##NAME##_t* that)                { ch_word result = vector_eq(this->_vector, that->_vector); _update_##NAME(this); return result;}\
static TYPE* _off_##NAME(ch_vector_##NAME##_t* this, ch_word idx)                            { TYPE* result = (TYPE*)vector_off(this->_vector, idx); _update_##NAME(this); return result; }\
static TYPE* _forward_##NAME(ch_vector_##NAME##_t* this, TYPE* ptr, ch_word amount)           { TYPE* result = (TYPE*)vector_forward(this->_vector, (void*)ptr, amount); _update_##NAME(this); return result; }\
static TYPE* _back_##NAME(ch_vector_##NAME##_t* this, TYPE* ptr, ch_word amount)              { TYPE* result =  (TYPE*)vector_back(this->_vector, (void*)ptr, amount); _update_##NAME(this); return result; }\
static TYPE* _next_##NAME(ch_vector_##NAME##_t* this, TYPE* ptr)                              { TYPE* result = (TYPE*)_forward_##NAME(this, ptr, 1); _update_##NAME(this); return result; }\
static TYPE* _prev_##NAME(ch_vector_##NAME##_t* this, TYPE* ptr)                              { TYPE* result = (TYPE*)_back_##NAME(this, ptr, 1); _update_##NAME(this); return result; }\
static TYPE* _find_##NAME(ch_vector_##NAME##_t* this, TYPE* begin, TYPE* end, TYPE value)       { TYPE* result = (TYPE*) vector_find(this->_vector, (void*)begin, (void*)end, &value); _update_##NAME(this); return result; }\
static void _sort_##NAME(ch_vector_##NAME##_t* this)                                        { vector_sort(this->_vector); _update_##NAME(this); }\
static TYPE* _push_front_##NAME(ch_vector_##NAME##_t* this, TYPE value)                       { TYPE* result = (TYPE*) vector_push_front(this->_vector, &value); _update_##NAME(this); return result; }\
static TYPE* _push_back_##NAME(ch_vector_##NAME##_t* this, TYPE value)                        { TYPE* result = (TYPE*) vector_push_back(this->_vector, &value); _update_##NAME(this); return result; }\
static TYPE* _insert_after_##NAME(ch_vector_##NAME##_t* this, TYPE* ptr, TYPE value)           { TYPE* result = (TYPE*) vector_insert_after(this->_vector, ptr, &value); _update_##NAME(this); return result; }\
static TYPE* _insert_before_##NAME(ch_vector_##NAME##_t* this, TYPE* ptr, TYPE value)          { TYPE* result = (TYPE*) vector_insert_before(this->_vector, ptr, &value); _update_##NAME(this); return result; }\
static TYPE* _remove_##NAME(ch_vector_##NAME##_t* this, TYPE* ptr)                            { TYPE* result = (TYPE*) vector_remove(this->_vector, ptr); _update_##NAME(this); return result; }\
static void _pop_front_##NAME(ch_vector_##NAME##_t* this)                                   { vector_pop_front(this->_vector); _update_##NAME(this); }\
static void _pop_back_##NAME(ch_vector_##NAME##_t* this)                                    { vector_pop_back(this->_vector); _update_##NAME(this); }\
static TYPE* _push_back_carray_##NAME(ch_vector_##NAME##_t* this, TYPE* carray, ch_word count){ TYPE* result =  vector_push_back_carray(this->_vector, (void*)carray, count); _update_##NAME(this); return result; }\
\
static void _delete_##NAME(ch_vector_##NAME##_t* this)\
{\
    if(this->_vector){\
        vector_delete(this->_vector);\
    }\
\
    free(this);\
}\
\
\
ch_vector_##NAME##_t* ch_vector_##NAME##_new(ch_word size, ch_word (*cmp)(TYPE* lhs, TYPE* rhs) )\
{\
\
    ch_vector_##NAME##_t* result = (ch_vector_##NAME##_t*)calloc(1,sizeof(ch_vector_##NAME##_t));\
    if(!result){\
        printf("Could not allocate memory for new vector structure. Giving upn");\
        return ((void *)0);\
    }\
\
    result->_vector = ch_vector_new(size, sizeof(TYPE), (cmp_void_f)cmp );\
\
\
    /*We have memory to play with, now do all the other assignments*/\
    result->resize                  = _resize_##NAME;\
    result->eq                      = _eq_##NAME;\
    result->off                     = _off_##NAME;\
    result->next                    = _next_##NAME;\
    result->prev                    = _prev_##NAME;\
    result->forward                 = _forward_##NAME;\
    result->back                    = _back_##NAME;\
    result->find                    = _find_##NAME;\
    result->sort                    = _sort_##NAME;\
    result->push_front              = _push_front_##NAME;\
    result->pop_front               = _pop_front_##NAME;\
    result->push_back               = _push_back_##NAME;\
    result->pop_back                = _pop_back_##NAME;\
    result->insert_after            = _insert_after_##NAME;\
    result->insert_before           = _insert_before_##NAME;\
    result->remove                  = _remove_##NAME;\
    result->push_back_carray        = _push_back_carray_##NAME;\
    result->delete                  = _delete_##NAME;\
\
\
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
#define define_ch_vector_cmp(NAME, TYPE) \
ch_word ch_vector_cmp_##NAME(TYPE* lhs, TYPE* rhs)\
{ \
    return ( *lhs == *rhs ? 0 : *lhs < *rhs ? -1 : 1); \
}

//Pointer comparison function
#define define_ch_vector_cmpp(NAME, TYPE) ch_word ch_vector_cmp_##NAME##p(TYPE* lhs, TYPE* rhs)\
{ \
    return ch_vector_cmp_##NAME(*lhs, *rhs);\
}


