// CamIO 2: array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include <stdlib.h>

#include "array_std_int.h"
#include "../log/log.h"

#include "../types/types.h"



/*Return the max number number of elements in the array*/
static ch_word _size(ch_array_int_t* this)
{
    return this->_array_backing_size;
}

/*Return the actual number of elements in the array*/
static ch_word _count(ch_array_int_t* this)
{
    return this->_array_backing_count;
}

/*Resize the array*/
static void _resize(ch_array_int_t* this, ch_word new_size)
{
    this->_array_backing = (int*)malloc(new_size * sizeof(int));
    if(!this->_array_backing){
        ch_log_error("Could not allocate memory for backing store\n");
        return;
    }

    this->_array_backing_size = new_size;

}

/*Take an index an fix it so that negative indexs are legal */
static inline ch_word range_fix(ch_array_int_t* this, ch_word idx)
{
    idx = idx < 0 ? idx + this->_array_backing_size : idx;

    if(idx >= 0 && idx < this->_array_backing_count && idx < this->_array_backing_size){
        return idx;
    }

    ch_log_error("Index (%li) is out of the valid range [%li,%li]", idx, -1 * this->_array_backing_size, this->_array_backing_size - 1 );
    return -1;

}


/*Return the element at a given offset, with bounds checking*/
static int* _off(ch_array_int_t* this, ch_word idx)
{

    idx = range_fix(this, idx);
    if(idx >= 0){
        return &this->_array_backing[idx];
    }

    return NULL;

}

/*Return an iterator at the given index*/
ch_array_int_iter_t* _iter_at(ch_array_int_t* this, ch_word idx)
{
    idx = range_fix(this, idx);
    if(idx >= 0 ){
        return ch_array_int_iter_new(this, idx);
    }

    return NULL;
}


/*Return an iterator at the given index*/
ch_array_int_iter_t* _start(ch_array_int_t* this)
{
    return -iter_at(this, 0);
}


ch_array_int_iter_t* _end(ch_array_int_t* this)
{
    return -iter_at(this, -1);
}



/*find the given value using the comparitor function*/
static ch_array_int_iter_t* _find(ch_array_int_t* this, ch_array_int_iter_t* begin, ch_array_int_iter_t* end, int value)
{
    for(ch_array_int_iter_t* it = begin; !it->eq(it,end); it->next(it)){
        if(this->_cmp(*it->value,value) == 0 ){
            return result
        }
    }

    return

}

/*sort into order given the comparitor function*/
static void _sort(ch_array_int_t* this)
{
    (void)this;
}

/*sort into reverse order given the comparitor function*/
static void _sort_reverse(ch_array_int_t* this)
{
    (void)this;
}


/* Put an element at the front of the array values, [WARN: In general this is very expensive for an array] */
static ch_array_int_iter_t* _push_front(ch_array_int_t* this, int value)
{
    (void)this;
    (void)value;
    return NULL;
}

/* Put an elemenet at the back of the arary values*/
static ch_array_int_iter_t* _push_back(ch_array_int_t* this, int value)
{
    (void)this;
    (void)value;
    return NULL;
}

/* Insert an element after the element given by iter*/
static ch_array_int_iter_t* _insert_after(ch_array_int_t* this, ch_array_int_iter_t* iter, int value)
{
    (void)this;
    (void)value;
    (void)iter;
    return NULL;
}

/* Insert an element befer the element giver by iter [WARN: In general this is very expensive for an array] */
static ch_array_int_iter_t* _insert_before(ch_array_int_t* this, ch_array_int_iter_t* iter, int value)
{
    (void)this;
    (void)iter;
    (void)value;
    return NULL;
}

/*Set the given iter to the given value */
static ch_array_int_iter_t* _assign(ch_array_int_t* this, ch_array_int_iter_t* iter, int value)
{
    (void)this;
    (void)iter;
    (void)value;
    return NULL;
}

/*Remove the given iter [WARN: In general this is very expensive], return an interator to the next item in the list */
static ch_array_int_iter_t*(_remove)(ch_array_int_t* this, ch_array_int_iter_t* iter)
{
    const ch_word bytes_to_move =

}


static void _delete(ch_array_int_t* this)
{
    if(this->_array_backing){
        free(this->_array_backing);
    }

    free(this);
}


ch_array_int_t* ch_array_int_new(ch_word size, int init_value, ch_word (*cmp)(int lhs, int rhs) )
{

    ch_array_int_t* result = (ch_array_int_t*)calloc(1,sizeof(ch_array_int_t));
    if(!result){
        ch_log_error("Could not allocate memory for new array structure. Giving up\n");
        return NULL;
    }

    result->_array_backing       = calloc(size,sizeof(int));
    if(!result->_array_backing){
        ch_log_error("Could not allocate memory for new array backing. Giving up\n");
        free(result);
        return NULL;
    }

    /*We have memory to play with, now do all the other assignments*/
    result->_array_backing_size  = size;
    result->_array_backing_count = 0;
    result->_cmp                 = cmp;

    result->size            = _size;
    result->count           = _count;
    result->resize          = _resize;
    result->off             = _off;
    result->iter_at         = _iter_at;
    result->find            = _find;
    result->sort            = _sort;
    result->sort_reverse    = _sort_reverse;
    result->push_front      = _push_front;
    result->push_back       = _push_back;
    result->insert_after    = _insert_after;
    result->insert_before   = _insert_before;
    result->assign          = _assign;
    result->remove          = _remove;
    result->delete          = _delete;

    for(ch_array_int_iter_t* it = result->start(result); it->value; it->next(it)){
        *it->value = init_value;
    }


    return result;
}


/**************************************************************************************************************************/


static ch_array_int_iter_t* _iter_forward(ch_array_int_iter_t* this, ch_word amount)
{

    if(this->value == NULL || this->_index < 0){
        return this;
    }

    if(this->_index + amount < this->_priv->_array_backing_count){
        this->_index += amount;
        this->value += amount;
    }
    else{
        this->value = NULL;
        this->_index = -1;
    }

    return this;
}


/* An iterator for looking at the whole array*/
static ch_array_int_iter_t* _iter_next(ch_array_int_iter_t* this)
{
    return _iter_forward(this,1);
}


static ch_array_int_iter_t* _iter_back(ch_array_int_iter_t* this, ch_word amount)
{
    if(this->value == NULL || this->_index < 0){
        return this;
    }

    if(this->_index - amount >= 0){
        this->_index -= amount;
        this->value -= amount;
    }
    else{
        this->value = NULL;
        this->_index = -1;
    }

    return this;
}


static ch_array_int_iter_t* _iter_prev(ch_array_int_iter_t* this)
{
     return _iter_backward(this, 1);
}


static ch_word _iter_eq(ch_array_int_iter_t* lhs, ch_array_int_iter_t rhs)
{
    if( (lhs->value == NULL || lhs->_index < 0) && (lhs->value == NULL || lhs->_index < 0) ){
        return 1;
    }

    return lhs->_index == rhs->_index;

}


static void _iter_delete(ch_array_int_iter_t* this)
{
    free(this);
}


/* The copy constructor */
ch_array_int_iter_t* ch_array_int_iter_new_from_it(ch_array_int_iter_t* it)
{
    ch_array_int_iter_t* result = (ch_array_int_iter_t*)calloc(1,sizeof(ch_array_int_iter_t));
    if(!result){
        ch_log_error("Could not allocate memory for new iterator\n");
        return NULL;
    }

    memcopy(result,it,sizeof(ch_array_int_iter_t));
    return result;
}

/* The regular constructor */
ch_array_int_iter_t* ch_array_int_iter_new(ch_array_int_t* array, ch_word off)
{
    ch_array_int_iter_t* result = (ch_array_int_iter_t*)calloc(1,sizeof(ch_array_int_iter_t));
    if(!result){
        ch_log_error("Could not allocate memory for new iterator\n");
        return NULL;
    }

    result->_index  = off;
    result->value   =  off < 0 ? NULL : &array->_array_backing[off];
    result->_priv   = array;
    result->delete  = _iter_delete;
    result->next    = _iter_next;
    result->prev    = _iter_prev;
    result->forward = _iter_forward;
    result->back    = _iter_back;
    result->eq      = _iter_eq;

    return result;
}


