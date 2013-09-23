/*
 * linked_list_std.c
 *
 *  Created on: Sep 23, 2013
 *      Author: mgrosvenor
 */

#include "linked_list.h"
#include "linked_list_std.h"
#include "linked_list_typed_define_template.h"

//define_ch_llist(UINT64, u64)

static inline void _update_UINT64(ch_llist_UINT64_t* this)
{
    this->count = this->_llist->count;
}

static inline ch_llist_UINT64_it _to_UINT64_it(const ch_llist_it* rhs)
{
    ch_llist_UINT64_it result = { ._node = rhs->_node, .value = (u64*)(rhs->value) };
    return result;
}

static inline ch_llist_it _from_UINT64_it(const ch_llist_UINT64_it* rhs)
{
    ch_llist_it result = { ._node = rhs->_node, .value = rhs->value };
    return result;
}

static ch_llist_UINT64_it  _off_UINT64(ch_llist_UINT64_t* this, ch_word idx)
{
    const ch_llist_it result = llist_off(this->_llist, idx);
    return _to_UINT64_it(&result);
}

static ch_llist_UINT64_it _first_UINT64(ch_llist_UINT64_t* this)
{
    const ch_llist_it result = llist_first(this->_llist);
    return _to_UINT64_it(&result);
}

/*Get the last entry*/
static ch_llist_UINT64_it _last_UINT64(ch_llist_UINT64_t* this)
{
    const ch_llist_it result = llist_last(this->_llist);
    return _to_UINT64_it(&result);
}

/*Get the end*/
static ch_llist_UINT64_it _end_UINT64(ch_llist_UINT64_t* this){
    const ch_llist_it result = llist_end(this->_llist);
    return _to_UINT64_it(&result);
}

static void  _forward_UINT64(ch_llist_UINT64_t* this, ch_llist_UINT64_it* itr, ch_word amount)
{
    ch_llist_it base_itr = _from_UINT64_it(itr);
    llist_forward(this->_llist, &base_itr, amount);
    *itr = _to_UINT64_it(&base_itr);
}

static void  _back_UINT64(ch_llist_UINT64_t* this, ch_llist_UINT64_it* itr, ch_word amount)
{
    ch_llist_it base_itr = _from_UINT64_it(itr);
    llist_back(this->_llist, &base_itr, amount);
    *itr = _to_UINT64_it(&base_itr);
}

static void  _next_UINT64(ch_llist_UINT64_t* this, ch_llist_UINT64_it* itr)
{
    _forward_UINT64(this, itr, 1);
}

static void  _prev_UINT64(ch_llist_UINT64_t* this, ch_llist_UINT64_it* itr)
{
    _back_UINT64(this, itr, 1);
}

static ch_llist_UINT64_it  _find_UINT64(ch_llist_UINT64_t* this, ch_llist_UINT64_it* begin, ch_llist_UINT64_it* end, u64 value)
{
    ch_llist_it base_begin  = _from_UINT64_it(begin);
    ch_llist_it base_end    = _from_UINT64_it(end);
    ch_llist_it result      = llist_find(this->_llist, &base_begin, &base_end, &value);
    return _to_UINT64_it(&result);
}


static void _sort_UINT64(ch_llist_UINT64_t* this)
{
    llist_sort(this->_llist);
}


static ch_llist_UINT64_it  _push_front_UINT64(ch_llist_UINT64_t* this, u64 value)
{
    ch_llist_it result = llist_push_front(this->_llist, &value);
    _update_UINT64(this);
    return _to_UINT64_it(&result);
}

static ch_llist_UINT64_it  _push_back_UINT64(ch_llist_UINT64_t* this, u64 value)
{
    ch_llist_it result = llist_push_back(this->_llist, &value);
    _update_UINT64(this);
    return _to_UINT64_it(&result);
}

static ch_llist_UINT64_it* _insert_after_UINT64(ch_llist_UINT64_t* this, ch_llist_UINT64_it* itr, u64 value)
{
    ch_llist_it base_itr = _from_UINT64_it(itr);
    ch_llist_it* result  =  llist_insert_after(this->_llist, &base_itr, &value);
    _update_UINT64(this);
    *itr  = _to_UINT64_it(result);
    return itr;

}

static ch_llist_UINT64_it*  _insert_before_UINT64(ch_llist_UINT64_t* this, ch_llist_UINT64_it* itr, u64 value)
{
    ch_llist_it base_itr = _from_UINT64_it(itr);
    ch_llist_it* result   =  llist_insert_before(this->_llist, &base_itr, &value);
    _update_UINT64(this);
    *itr  = _to_UINT64_it(result);
    return itr;
}

static ch_llist_UINT64_it  _remove_UINT64(ch_llist_UINT64_t* this, ch_llist_UINT64_it* itr)
{
    ch_llist_it base_itr = _from_UINT64_it(itr);
    ch_llist_it result  =  llist_remove(this->_llist, &base_itr);
    _update_UINT64(this);
    return _to_UINT64_it(&result);
}

static ch_llist_UINT64_it _pop_front_UINT64(ch_llist_UINT64_t* this)
{
    ch_llist_it result = llist_pop_front(this->_llist);
    _update_UINT64(this);
    return _to_UINT64_it(&result);
}


static ch_llist_UINT64_it _pop_back_UINT64(ch_llist_UINT64_t* this)
{
    ch_llist_it result = llist_pop_back(this->_llist);
    _update_UINT64(this);
    return _to_UINT64_it(&result);
}

static ch_llist_UINT64_it  _push_back_carray_UINT64(ch_llist_UINT64_t* this, const u64* carray, ch_word count)
{
    ch_llist_it result =  llist_push_back_carray(this->_llist, (void*)carray, count);
    _update_UINT64(this);
    return _to_UINT64_it(&result);
}

static ch_word _eq_UINT64(ch_llist_UINT64_t* this, ch_llist_UINT64_t* that)
{
    return llist_eq(this->_llist, that->_llist);
}


static void _delete_UINT64(ch_llist_UINT64_t* this)
{
    if(this->_llist){
        llist_delete(this->_llist);
    }

    free(this);
}


ch_llist_UINT64_t* ch_llist_UINT64_new(ch_word(*cmp)(u64* lhs, u64* rhs) )
{

    ch_llist_UINT64_t* result = (ch_llist_UINT64_t*)calloc(1,sizeof(ch_llist_UINT64_t));
    if(!result){
        printf("Could not allocate memory for new llist structure. Giving upn");
        return ((void *)0);
    }

    result->_llist = ch_llist_new(sizeof(u64), (cmp_void_f)cmp );


    /*We have memory to play with, now do all the other assignments*/

    result->off                     = _off_UINT64;
    result->first                   = _first_UINT64;
    result->last                    = _last_UINT64;
    result->end                     = _end_UINT64;

    result->next                    = _next_UINT64;
    result->prev                    = _prev_UINT64;
    result->forward                 = _forward_UINT64;
    result->back                    = _back_UINT64;

    result->push_front              = _push_front_UINT64;
    result->pop_front               = _pop_front_UINT64;
    result->push_back               = _push_back_UINT64;
    result->pop_back                = _pop_back_UINT64;
    result->insert_after            = _insert_after_UINT64;
    result->insert_before           = _insert_before_UINT64;

    result->remove                  = _remove_UINT64;

    result->push_back_carray        = _push_back_carray_UINT64;
    result->delete                  = _delete_UINT64;


    /*Fail hard and early if the compare function is NULL*/
    if(cmp){
        result->eq                      = _eq_UINT64;
        result->find                    = _find_UINT64;
        result->sort                    = _sort_UINT64;
    }

    _update_UINT64(result);

    return result;
}

