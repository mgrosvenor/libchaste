#ifndef LINKED_LIST_TYPED_TEMPLATE_DEFINE_H_
#define LINKED_LIST_TYPED_TEMPLATE_DEFINE_H_

#include "linked_list.h"
#include "linked_list_std.h"

#include "../../utils/util.h"
#include "../../types/types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "linked_list_std.h"
#include "linked_list_typed_define_template.h"

#define define_ch_llist(NAME, TYPE)\
\
static inline void _update_##NAME(ch_llist_##NAME##_t* this)\
{\
    this->count = this->_llist->count;\
}\
\
static inline ch_llist_##NAME##_it _to_##NAME##_it(const ch_llist_it* rhs)\
{\
    ch_llist_##NAME##_it result = { ._node = rhs->_node, .value = (TYPE*)(rhs->value) };\
    return result;\
}\
\
static inline ch_llist_it _from_##NAME##_it(const ch_llist_##NAME##_it* rhs)\
{\
    ch_llist_it result = { ._node = rhs->_node, .value = rhs->value };\
    return result;\
}\
\
static ch_llist_##NAME##_it  _off_##NAME(ch_llist_##NAME##_t* this, ch_word idx)\
{\
    const ch_llist_it result = llist_off(this->_llist, idx);\
    return _to_##NAME##_it(&result);\
}\
\
static ch_llist_##NAME##_it _first_##NAME(ch_llist_##NAME##_t* this)\
{\
    const ch_llist_it result = llist_first(this->_llist);\
    return _to_##NAME##_it(&result);\
}\
\
/*Get the last entry*/\
static ch_llist_##NAME##_it _last_##NAME(ch_llist_##NAME##_t* this)\
{\
    const ch_llist_it result = llist_last(this->_llist);\
    return _to_##NAME##_it(&result);\
}\
\
/*Get the end*/\
static ch_llist_##NAME##_it _end_##NAME(ch_llist_##NAME##_t* this){\
    const ch_llist_it result = llist_end(this->_llist);\
    return _to_##NAME##_it(&result);\
}\
\
static void  _forward_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it* itr, ch_word amount)\
{\
    ch_llist_it base_itr = _from_##NAME##_it(itr);\
    llist_forward(this->_llist, &base_itr, amount);\
    *itr = _to_##NAME##_it(&base_itr);\
}\
\
static void  _back_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it* itr, ch_word amount)\
{\
    ch_llist_it base_itr = _from_##NAME##_it(itr);\
    llist_back(this->_llist, &base_itr, amount);\
    *itr = _to_##NAME##_it(&base_itr);\
}\
\
static void  _next_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it* itr)\
{\
    _forward_##NAME(this, itr, 1);\
}\
\
static void  _prev_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it* itr)\
{\
    _back_##NAME(this, itr, 1);\
}\
\
static ch_llist_##NAME##_it  _find_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it* begin, ch_llist_##NAME##_it* end, TYPE value)\
{\
    ch_llist_it base_begin  = _from_##NAME##_it(begin);\
    ch_llist_it base_end    = _from_##NAME##_it(end);\
    ch_llist_it result      = llist_find(this->_llist, &base_begin, &base_end, &value);\
    return _to_##NAME##_it(&result);\
}\
static ch_llist_##NAME##_it  _find_first_##NAME(ch_llist_##NAME##_t* this, TYPE value)\
{\
    ch_llist_it result      = llist_find_first(this->_llist, &value);\
    return _to_##NAME##_it(&result);\
}\
static ch_llist_##NAME##_it  _find_next_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it* begin, TYPE value)\
{\
    ch_llist_it base_begin  = _from_##NAME##_it(begin);\
    ch_llist_it result      = llist_find_next(this->_llist, &base_begin, &value);\
    return _to_##NAME##_it(&result);\
}\
\
\
static void _sort_##NAME(ch_llist_##NAME##_t* this)\
{\
    llist_sort(this->_llist);\
}\
\
\
static ch_llist_##NAME##_it  _push_front_##NAME(ch_llist_##NAME##_t* this, TYPE value)\
{\
    ch_llist_it result = llist_push_front(this->_llist, &value);\
    _update_##NAME(this);\
    return _to_##NAME##_it(&result);\
}\
\
static ch_llist_##NAME##_it  _push_back_##NAME(ch_llist_##NAME##_t* this, TYPE value)\
{\
    ch_llist_it result = llist_push_back(this->_llist, &value);\
    _update_##NAME(this);\
    return _to_##NAME##_it(&result);\
}\
\
static ch_llist_##NAME##_it* _insert_after_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it* itr, TYPE value)\
{\
    ch_llist_it base_itr = _from_##NAME##_it(itr);\
    ch_llist_it* result  =  llist_insert_after(this->_llist, &base_itr, &value);\
    _update_##NAME(this);\
    *itr  = _to_##NAME##_it(result);\
    return itr;\
\
}\
\
static ch_llist_##NAME##_it*  _insert_before_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it* itr, TYPE value)\
{\
    ch_llist_it base_itr = _from_##NAME##_it(itr);\
    ch_llist_it* result   =  llist_insert_before(this->_llist, &base_itr, &value);\
    _update_##NAME(this);\
    *itr  = _to_##NAME##_it(result);\
    return itr;\
}\
\
static ch_llist_##NAME##_it  _remove_it_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_it* itr)\
{\
    ch_llist_it base_itr = _from_##NAME##_it(itr);\
    ch_llist_it result  =  llist_remove_it(this->_llist, &base_itr);\
    _update_##NAME(this);\
    return _to_##NAME##_it(&result);\
}\
static ch_llist_##NAME##_it  _remove_all_##NAME(ch_llist_##NAME##_t* this, TYPE value)\
{\
    ch_llist_it result  =  llist_remove_all(this->_llist, &value);\
    _update_##NAME(this);\
    return _to_##NAME##_it(&result);\
}\
\
static ch_llist_##NAME##_it _pop_front_##NAME(ch_llist_##NAME##_t* this)\
{\
    ch_llist_it result = llist_pop_front(this->_llist);\
    _update_##NAME(this);\
    return _to_##NAME##_it(&result);\
}\
\
\
static ch_llist_##NAME##_it _pop_back_##NAME(ch_llist_##NAME##_t* this)\
{\
    ch_llist_it result = llist_pop_back(this->_llist);\
    _update_##NAME(this);\
    return _to_##NAME##_it(&result);\
}\
\
static ch_llist_##NAME##_it  _push_back_carray_##NAME(ch_llist_##NAME##_t* this, const TYPE* carray, ch_word count)\
{\
    ch_llist_it result =  llist_push_back_carray(this->_llist, (void*)carray, count);\
    _update_##NAME(this);\
    return _to_##NAME##_it(&result);\
}\
\
static ch_word _eq_##NAME(ch_llist_##NAME##_t* this, ch_llist_##NAME##_t* that)\
{\
    return llist_eq(this->_llist, that->_llist);\
}\
\
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
ch_llist_##NAME##_t* ch_llist_##NAME##_new(ch_word(*cmp)(TYPE* lhs, TYPE* rhs) )\
{\
\
    ch_llist_##NAME##_t* result = (ch_llist_##NAME##_t*)calloc(1,sizeof(ch_llist_##NAME##_t));\
    if(!result){\
        printf("Could not allocate memory for new llist structure. Giving upn");\
        return ((void *)0);\
    }\
\
    result->_llist = ch_llist_new(sizeof(TYPE), (cmp_void_f)cmp );\
\
\
    /*We have memory to play with, now do all the other assignments*/\
\
    result->off                     = _off_##NAME;\
    result->first                   = _first_##NAME;\
    result->last                    = _last_##NAME;\
    result->end                     = _end_##NAME;\
\
    result->next                    = _next_##NAME;\
    result->prev                    = _prev_##NAME;\
    result->forward                 = _forward_##NAME;\
    result->back                    = _back_##NAME;\
\
    result->push_front              = _push_front_##NAME;\
    result->pop_front               = _pop_front_##NAME;\
    result->push_back               = _push_back_##NAME;\
    result->pop_back                = _pop_back_##NAME;\
    result->insert_after            = _insert_after_##NAME;\
    result->insert_before           = _insert_before_##NAME;\
\
    result->remove_it               = _remove_it_##NAME;\
    result->remove_all              = _remove_all_##NAME;\
\
    result->push_back_carray        = _push_back_carray_##NAME;\
    result->delete                  = _delete_##NAME;\
\
\
    /*Fail hard and early if the compare function is NULL*/\
    if(cmp){\
        result->eq                      = _eq_##NAME;\
        result->find                    = _find_##NAME;\
        result->find_first              = _find_first_##NAME;\
        result->find_next               = _find_next_##NAME;\
        result->sort                    = _sort_##NAME;\
    }\
\
    _update_##NAME(result);\
\
    return result;\
}\
\


//Regular comparison function
#define define_ch_llist_cmp(NAME, TYPE) \
ch_word ch_llist_cmp_##NAME(TYPE* lhs, TYPE* rhs)\
{ \
    return ( *lhs == *rhs ? 0 : *lhs < *rhs ? -1 : 1); \
}

//Pointer comparison function
#define define_ch_llist_cmpp(NAME, TYPE) ch_word ch_llist_cmp_##NAME##p(TYPE* lhs, TYPE* rhs)\
{ \
    return ch_llist_cmp_##NAME(*lhs, *rhs);\
}

#define define_ch_llist_compare(NAME, TYPE) ch_word ch_llist_cmp_##NAME(TYPE* lhs, TYPE* rhs)


#endif //LINKED_LIST_TYPED_TEMPLATE_DEFINE_H_
