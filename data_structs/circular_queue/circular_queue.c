/*
 * This is the implementation of a circular buffer based queue data structure. The data structure provides a fixed sized
 * queue with constant time push and pop operations (much like a linked listed) but with out runtime memory allocation
 * costs. The structure provides the ability to push items on to the queue, to "use" items from that queue and to release
 * items from the queue. All operations are O(c) time with the size of the queue. The structure also provides the ability to
 * push an array of times on to the queue which is O(n) with the size of the array.
 *
 * cbq.c
 *
 *  Created on: July 22, 2015
 *      Author: mgrosvenor
 */


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "../../utils/util.h"
#include "../../types/types.h"
#include "../array/array.h"
#include "circular_queue.h"


#include <assert.h>

//Remove element from the front of the queue.
int cbq_pop_front(ch_cbq_t* this)
{
    if(this->count == 0){//Nothing to pop!
        return -1;
    }

    assert(this->in_use <= this->count);
    assert(this->in_use <= this->size);
    assert(this->in_use >= 0);

    this->count--;
    this->_release_index++;
    if(this->_release_index >= this->size){
        this->_release_index = 0;
    }

    if(this->in_use){
        this->in_use--;
    }

    return 0;
}

//Return pointer to the front of the buffer do not remove item
void* cbq_peek_front(ch_cbq_t* this)
{
    if(this->count == 0){
        return NULL; //Nothing to peek!
    }

    assert(this->in_use <= this->count);
    assert(this->in_use <= this->size);
    assert(this->in_use >= 0);

    return array_off(this->_array, this->_release_index);
}

//Get a pointer to the front of the queue and mark it as "in use".
void* cbq_use_front(ch_cbq_t* this)
{
    assert(this->in_use <= this->count);
    assert(this->in_use <= this->size);
    assert(this->in_use >= 0);


    if(this->count == 0){
        return NULL; //Nothing to use!
    }

    if(this->in_use >= this->count){
        return NULL; //Everything in use
    }

    //Reset in case the release pointer has moved on us
    if(this->in_use == 0){
        this->_use_next_index = this->_release_index;
    }

    void* result = array_off(this->_array,this->_use_next_index);
    if(!result){
        //DBG("Error getting item from array at index %lli\n", this->_use_next_index);
        return NULL;
    }

    this->_use_next_index++;
    if(this->_use_next_index >= this->size){
        this->_use_next_index = 0;
    }
    this->in_use++;

    return result;

}


//Get a pointer to the front of the queue and mark it as "out of use".
void cbq_unuse_front(ch_cbq_t* this)
{

    assert(this->in_use <= this->count);
    assert(this->in_use <= this->size);
    assert(this->in_use >= 0);

    if(this->count == 0){
        return; //Nothing to unuse!
    }

    if(this->in_use == 0){
        return; //Nothing to unuse!
    }

    this->in_use--;
    this->_use_next_index--;
    if(this->_use_next_index < 0){
        this->_use_next_index = this->size -1;
    }
    //printf("%s:%i #### In use now=%lli\n", __FUNCTION__, __LINE__,  this->in_use);

}


/*Assign at most size elements from the C cbq*/
void* cbq_push_back_carray(ch_cbq_t* this, void* carray, ch_word* len_io)
{

    ch_word len = *len_io;
    if(len == 0){
        *len_io = -1;
        return NULL;
    }

    //Trim the len to the amount that we can handle
    if(len > this->size - this->count){
        len = this->size - this->count;
    }

    //Copy items up until the wrap around point
    void* b4_start = array_off(this->_array, this->_add_next_index);
    const ch_word num_b4    = MIN(this->size - this->_add_next_index,len);
    memcpy(b4_start, carray, num_b4 * this->_array->_element_size );
    this->_add_next_index += num_b4;

    //Copy anything left over
    if(num_b4 < len){
        const ch_word num_start = len - num_b4;
        void* start = array_off(this->_array, 0);
        memcpy(start, (char*)carray + num_b4 * this->_array->_element_size, num_start * this->_array->_element_size );
        this->_add_next_index = num_start;
    }

    if(this->_add_next_index >= this->size){
        this->_add_next_index = this->_add_next_index - this->size;
    }

    //now increase the the count
    this->count += len;

    *len_io = len;

    assert(this->in_use <= this->count);
    assert(this->in_use <= this->size);
    assert(this->in_use >= 0);

    return b4_start;
}

/* Put an element at the back of the arary values*/
void* cbq_push_back(ch_cbq_t* this, void* value)
{
    ch_word len = 1; //Throw away value
    void* result = cbq_push_back_carray(this,value,&len);
    if(len != 1){
        return NULL;
    }
    return result;
}


ch_cbq_t* ch_cbq_new(ch_word size, ch_word element_size)
{

    ch_cbq_t* result = (ch_cbq_t*)calloc(1,sizeof(ch_cbq_t));
    if(!result){
        printf("Could not allocate memory for new cbq structure. Giving up\n");
        return NULL;
    }

    result->_array       = ch_array_new(size, element_size, NULL);

    /*We have memory to play with, now do all the other assignments*/
    result->size          = result->_array->size;
    result->count         = 0;

    return result;
}


void cbq_delete(ch_cbq_t* this)
{
    if(this->_array){
        array_delete(this->_array);
    }

    free(this);
}


