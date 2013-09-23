#ifndef VECTOR_TYPED_TEMPLATE_DEFINE_H_
#define VECTOR_TYPED_TEMPLATE_DEFINE_H_

#include "linked_list.h"
#include "linked_list_std.h"

#include "../../utils/util.h"
#include "../../types/types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define define_ch_llist(NAME,TYPE)\
\
static void _update_##NAME(ch_llist_##NAME##_t* this)\
{\
    this->count = this->_llist->count;\
}\
\
static void _update_##NAME(ch_llist_##NAME##_t* this)\
{\
    this->count = this->_llist->count;\
}\
\
static ch_llist_##NAME##_it _off_##NAME(ch_llist_##NAME##_t* this, ch_word idx)                        { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it)llist_off(this->_llist, idx); _update_##NAME(this); return result; }\
\
ch_llist_##NAME##_it (*first)(ch_llist_t* this) {  llist_first(this->_llist}\
ch_llist_##NAME##_it (*last)(ch_llist_t* this); /*Get the last entry*/\
ch_llist_##NAME##_it (*end)(ch_llist_t* this); /*Get the end*/\
\
static ch_llist_##NAME##_it _forward_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it ptr, ch_word amount)           { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it)llist_forward(this->_llist, (void*)ptr, amount); _update_##NAME(this); return result; }\
static ch_llist_##NAME##_it _back_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it ptr, ch_word amount)              { ch_llist_##NAME##_it result =  (ch_llist_##NAME##_it)llist_back(this->_llist, (void*)ptr, amount); _update_##NAME(this); return result; }\
static ch_llist_##NAME##_it _next_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it ptr)                              { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it)_forward_##NAME(this, ptr, 1); _update_##NAME(this); return result; }\
static ch_llist_##NAME##_it _prev_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it ptr)                              { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it)_back_##NAME(this, ptr, 1); _update_##NAME(this); return result; }\
static ch_llist_##NAME##_it _find_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it begin, ch_llist_##NAME##_it end, TYPE value)       { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it) llist_find(this->_llist, (void*)begin, (void*)end, &value); _update_##NAME(this); return result; }\
static void _sort_##NAME(ch_llist_##NAME##_t* this)                                        { llist_sort(this->_llist); _update_##NAME(this); }\
static ch_llist_##NAME##_it _push_front_##NAME(ch_llist_##NAME##_t* this, TYPE value)                       { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it) llist_push_front(this->_llist, &value); _update_##NAME(this); return result; }\
static ch_llist_##NAME##_it _push_back_##NAME(ch_llist_##NAME##_t* this, TYPE value)                        { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it) llist_push_back(this->_llist, &value); _update_##NAME(this); return result; }\
static ch_llist_##NAME##_it _insert_after_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it ptr, TYPE value)           { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it) llist_insert_after(this->_llist, ptr, &value); _update_##NAME(this); return result; }\
static ch_llist_##NAME##_it _insert_before_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it ptr, TYPE value)          { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it) llist_insert_before(this->_llist, ptr, &value); _update_##NAME(this); return result; }\
static ch_llist_##NAME##_it _remove_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it ptr)                            { ch_llist_##NAME##_it result = (ch_llist_##NAME##_it) llist_remove(this->_llist, ptr); _update_##NAME(this); return result; }\
static void _pop_front_##NAME(ch_llist_##NAME##_t* this)                                   { llist_pop_front(this->_llist); _update_##NAME(this); }\
static void _pop_back_##NAME(ch_llist_##NAME##_t* this)                                    { llist_pop_back(this->_llist); _update_##NAME(this); }\
static ch_llist_##NAME##_it _push_back_carray_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it carray, ch_word count){ ch_llist_##NAME##_it result =  llist_push_back_carray(this->_llist, (void*)carray, count); _update_##NAME(this); return result; }\
static ch_word _eq_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_t* that)              { ch_word result = llist_eq(this->_llist, that->_llist); _update_##NAME(this); return result;}\
\
static void _delete_##NAME(ch_llist_##NAME##_t* this)\
{\
    if(this->_llist){\
        llist_delete(this->_llist);\
    }\
\
    free(this);\
}\
\
\
ch_llist_##NAME##_t* ch_llist_##NAME##_new(ch_word size, ch_word (*cmp)(ch_llist_##NAME##_it lhs, ch_llist_##NAME##_it rhs) )\
{\
\
    ch_llist_##NAME##_t* result = (ch_llist_##NAME##_t*)calloc(1,sizeof(ch_llist_##NAME##_t));\
    if(!result){\
        printf("Could not allocate memory for new llist structure. Giving upn");\
        return ((void *)0);\
    }\
\
    result->_llist = ch_llist_new(size, sizeof(TYPE), (cmp_void_f)cmp );\
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
#define define_ch_llist_cmp(NAME, TYPE) \
ch_word ch_llist_cmp_##NAME(ch_llist_##NAME##_it lhs, ch_llist_##NAME##_it rhs)\
{ \
    return ( *lhs == *rhs ? 0 : *lhs < *rhs ? -1 : 1); \
}

//Pointer comparison function
#define define_ch_llist_cmpp(NAME, TYPE) ch_word ch_llist_cmp_##NAME##p(ch_llist_##NAME##_it lhs, ch_llist_##NAME##_it rhs)\
{ \
    return ch_llist_cmp_##NAME(*lhs, *rhs);\
}

#endif //VECTOR_TYPED_TEMPLATE_DEFINE_H_
