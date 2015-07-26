/*
 * cbuff.c
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
#include "circular_buffer.h"




//Remove element from the front of the queue.
int cbuff_pop_front(ch_cbuff_t* this)
{
    if(this->count == 0){//Nothing to pop!
        return -1;
    }

    this->_read_index++;
    this->count--;

    if(this->_read_index >= this->size){
        this->_read_index = 0;
    }

    return 0;
}

//Return pointer to the front of the buffer do not remove item
void* cbuff_peek_front(ch_cbuff_t* this)
{
    if(this->count == 0){
        return NULL; //Nothing to peek!
    }

    //void* val =array_off(this->_array, this->_read_index);
    //printf("%s:%i #### Getting items at %lli, pointer=%p val=0x%016lx\n", __FUNCTION__, __LINE__,  this->_read_index, val, *(size_t*)val);
    return array_off(this->_array, this->_read_index);
}

//Get a pointer to the front of the queue and mark it as "in use".
void* cbuff_use_front(ch_cbuff_t* this)
{
    if(this->count == 0){
        return NULL; //Nothing to use!
    }

    if(this->in_use >= this->count){
        return NULL; //Everything in use
    }

    void* result = array_off(this->_array,this->_used_index);
    this->_used_index = this->_used_index + 1 < this->count ? this->_used_index + 1 : 0;
    this->in_use++;
    //printf("%s:%i #### In use now=%lli\n", __FUNCTION__, __LINE__,  this->in_use);
    return result;

}


//Get a pointer to the front of the queue and mark it as "out of use".
void cbuff_unuse_front(ch_cbuff_t* this)
{
    if(this->count == 0){
        return; //Nothing to unuse!
    }

    if(this->in_use == 0){
        return; //Nothing to unuse!
    }

    this->in_use--;
    //printf("%s:%i #### In use now=%lli\n", __FUNCTION__, __LINE__,  this->in_use);

}


void* cbuff_prev(ch_cbuff_t* this, void* curr)
{
    //Calculate the current offset
    const ch_word idx = array_get_idx(this->_array, curr);
    if(idx == INT_MIN){
        return NULL;
    }

    if(this->count == 0){
        return NULL; //This is nothing to get prev off!
    }

    //Special case when the buffer is full
    if(this->_write_index == this->_read_index){
        if(!this->count == this->size){
            printf("Assertion failed! How did we get into this state???\n");
            return NULL;
        }

        return array_off(this->_array, idx -1 ); //Array knows how to handle wrap around
    }


    if(idx > this->_write_index || idx < this->_read_index + 1){
        printf("Your points is out of the space! idx=%lli write_max=%lli, read_min=%lli\n",
                idx, this->_write_index, this->_read_index + 1
        );

        return NULL;
    }

    return array_off(this->_array, idx -1 ); //Array knows how to handle wrap around

}


void cbuff_delete(ch_cbuff_t* this)
{
    if(this->_array){
        array_delete(this->_array);
    }

    free(this);
}


/*Assign at most size elements from the C cbuff*/
int cbuff_push_back_carray(ch_cbuff_t* this, void* carray, ch_word len)
{

    //printf("%s:%i #### Pushing back %lli items at %p\n", __FUNCTION__, __LINE__, len, carray);
    //printf("%s:%i #### Pushing count=%lli size=%lli\n", __FUNCTION__, __LINE__, this->count, this->size);
    if(len == 0){
        return CH_CBUFF_TOOFEW;
    }

    if(this->count + len > this->size){
        return CH_CBUFF_TOOMANY;
    }


    //Copy items up until the wrap around point
    void* b4_start = array_off(this->_array, this->_write_index);
    const ch_word num_b4    = MIN(this->size - this->_write_index,len);
    //printf("%s:%i #### num_b4=%lli, idx=%lli bytes=%lli start=%p val=0x%016lx\n", __FUNCTION__, __LINE__, num_b4, this->_write_index, num_b4 * this->_array->_element_size, b4_start, *(size_t*)carray);
    memcpy(b4_start, carray, num_b4 * this->_array->_element_size );
    this->_write_index += num_b4;

    //Copy anything left over
    if(num_b4 < len){
        const ch_word num_start = len - num_b4;
        void* start = array_off(this->_array, 0);
        memcpy(start, (char*)carray + num_b4, num_start * this->_array->_element_size );
        this->_write_index = num_start;
    }

    if(this->_write_index >= this->size){
        this->_write_index = this->_write_index - this->size;
    }

    //now increase the the count
    this->count += len;
    //printf("%s:%i #### count=%lli\n", __FUNCTION__, __LINE__, this->count);

    return 0;
}

/* Put an element at the back of the arary values*/
int cbuff_push_back(ch_cbuff_t* this, void* value)
{
    return cbuff_push_back_carray(this,value,1);
}


ch_cbuff_t* ch_cbuff_new(ch_word size, ch_word element_size)
{

    ch_cbuff_t* result = (ch_cbuff_t*)calloc(1,sizeof(ch_cbuff_t));
    if(!result){
        printf("Could not allocate memory for new cbuff structure. Giving up\n");
        return NULL;
    }

    result->_array       = ch_array_new(size, element_size, NULL);

    /*We have memory to play with, now do all the other assignments*/
    result->size                    = result->_array->size;
    result->count                   = 0;

    return result;
}

