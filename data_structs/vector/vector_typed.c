#include "vector.h"
#include "vector_typed.h"

#include "../../utils/util.h"
#include "../../types/types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define define_vector(i64)

static void _update_i64(ch_vector_i64_t* this)
{
    this->size  = this->_vector->size;
    this->first = this->_vector->first;
    this->last  = this->_vector->last;
    this->end   = this->_vector->end;
    this->count = this->_vector->count;
}

static void _resize_i64(ch_vector_i64_t* this, ch_word new_size)                    { vector_resize(this->_vector, new_size); _update_i64(this); }
static ch_word _eq_i64(ch_vector_i64_t* this, ch_vector_i64_t* that)                { ch_word result = vector_eq(this->_vector, that->_vector); _update_i64(this); return result;}
static i64* _off_i64(ch_vector_i64_t* this, ch_word idx)                            { i64* result = (i64*)vector_off(this->_vector, idx); _update_i64(this); return result; }
static i64* _forward_i64(ch_vector_i64_t* this, i64* ptr, ch_word amount)           { i64* result = (i64*)vector_forward(this->_vector, (void*)ptr, amount); _update_i64(this); return result; }
static i64* _back_i64(ch_vector_i64_t* this, i64* ptr, ch_word amount)              { i64* result =  (i64*)vector_back(this->_vector, (void*)ptr, amount); _update_i64(this); return result; }
static i64* _next_i64(ch_vector_i64_t* this, i64* ptr)                              { i64* result = (i64*)_forward_i64(this, ptr, 1); _update_i64(this); return result; }
static i64* _prev_i64(ch_vector_i64_t* this, i64* ptr)                              { i64* result = (i64*)_back_i64(this, ptr, 1); _update_i64(this); return result; }
static i64* _find_i64(ch_vector_i64_t* this, i64* begin, i64* end, i64 value)       { i64* result = (i64*) vector_find(this->_vector, (void*)begin, (void*)end, &value); _update_i64(this); return result; }
static void _sort_i64(ch_vector_i64_t* this)                                        { vector_sort(this->_vector); _update_i64(this); }
static i64* _push_front_i64(ch_vector_i64_t* this, i64 value)                       { i64* result = (i64*) vector_push_front(this->_vector, &value); _update_i64(this); return result; }
static i64* _push_back_i64(ch_vector_i64_t* this, i64 value)                        { i64* result = (i64*) vector_push_back(this->_vector, &value); _update_i64(this); return result; }
static i64* _insert_after_i64(ch_vector_i64_t* this, i64* ptr, i64 value)           { i64* result = (i64*) vector_insert_after(this->_vector, ptr, &value); _update_i64(this); return result; }
static i64* _insert_before_i64(ch_vector_i64_t* this, i64* ptr, i64 value)          { i64* result = (i64*) vector_insert_before(this->_vector, ptr, &value); _update_i64(this); return result; }
static i64* _remove_i64(ch_vector_i64_t* this, i64* ptr)                            { i64* result = (i64*) vector_remove(this->_vector, ptr); _update_i64(this); return result; }
static void _pop_front_i64(ch_vector_i64_t* this)                                   { vector_pop_front(this->_vector); _update_i64(this); }
static void _pop_back_i64(ch_vector_i64_t* this)                                    { vector_pop_back(this->_vector); _update_i64(this); }
static i64* _push_back_carray_i64(ch_vector_i64_t* this, i64* carray, ch_word count){ i64* result =  vector_push_back_carray(this->_vector, (void*)carray, count); _update_i64(this); return result; }

static void _delete_i64(ch_vector_i64_t* this)
{
    if(this->_vector){
        vector_delete(this->_vector);
    }

    free(this);
}


ch_vector_i64_t* ch_vector_i64_new(ch_word size, ch_word (*cmp)(i64* lhs, i64* rhs) )
{

    ch_vector_i64_t* result = (ch_vector_i64_t*)calloc(1,sizeof(ch_vector_i64_t));
    if(!result){
        printf("Could not allocate memory for new vector structure. Giving upn");
        return ((void *)0);
    }

    result->_vector = ch_vector_new(size, sizeof(i64), (cmp_void_f)cmp );


    /*We have memory to play with, now do all the other assignments*/
    result->resize                  = _resize_i64;
    result->eq                      = _eq_i64;
    result->off                     = _off_i64;
    result->next                    = _next_i64;
    result->prev                    = _prev_i64;
    result->forward                 = _forward_i64;
    result->back                    = _back_i64;
    result->find                    = _find_i64;
    result->sort                    = _sort_i64;
    result->push_front              = _push_front_i64;
    result->pop_front               = _pop_front_i64;
    result->push_back               = _push_back_i64;
    result->pop_back                = _pop_back_i64;
    result->insert_after            = _insert_after_i64;
    result->insert_before           = _insert_before_i64;
    result->remove                  = _remove_i64;
    result->push_back_carray        = _push_back_carray_i64;
    result->delete                  = _delete_i64;



    /*Fail hard and early if the compare function is NULL*/
    if(cmp){
        result->eq                      = _eq_i64;
        result->find                    = _find_i64;
        result->sort                    = _sort_i64;
    }

    _update_i64(result);

    return result;
}
