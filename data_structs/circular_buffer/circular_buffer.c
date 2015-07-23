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
        return -1; //Nothing to peek!
    }

    return array_off(this->_array, this->_read_index);
}

//Return a pointer to the front of the queue and mark it as "in use".
void* cbuff_use_front(ch_cbuff_t* this)
{
    if(this->count == 0){
        return NULL; //Nothing to use!
    }

    if(this->in_use >= this->count){
        return NULL; //Everything in use
    }

    void* result = array_off(this->_used_index);
    this->_used_index = this->_used_index + 1 < this->count ? this->_used_index + 1 : 0;
    this->in_use++;

    return result;

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
    if(len == 0){
        return CH_CBUFF_TOOFEW;
    }

    if(this->count + len > this->size){
        return CH_CBUFF_TOOMANY;
    }

    //Copy items up until the wrap around point
    void* b4_start = array_off(this->_array, this->_write_index);
    const ch_word num_b4    = MIN(this->size - this->_write_index,len);
    memcpy(b4_start, carray, num_b4 * this->_array->_element_size );
    this->_write_index += num_b4;

    //Copy anything left over
    if(num_b4 < len){
        const ch_word num_start = len - num_b4;
        void* start = array_off(this->_array, 0);
        memcpy(start, carray + num_b4, num_start * this->_array->_element_size );
        this->_write_index = num_start;
    }

    //now increase the the count
    this->count += len;

    return 0;
}

/* Put an element at the back of the arary values*/
void* cbuff_push_back(ch_cbuff_t* this, void* value)
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

