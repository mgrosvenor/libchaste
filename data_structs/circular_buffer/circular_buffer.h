/*
 * cbuff.h
 *
 *  Created on: July 22, 2015
 *      Author: mgrosvenor
 */

#ifndef CBUFF_H_
#define CBUFF_H_

#include "../../types/types.h"
#include "../array/array.h"

struct ch_cbuff;
typedef struct ch_cbuff ch_cbuff_t;

#define CH_CBUFF_TOOFEW (-1)
#define CH_CBUFF_TOOMANY (-2)

struct ch_cbuff{
    ch_word size;  //Return the max number number of elements in the cbuff list
    ch_word count;  //Return the actual number of elements in the cbuff
    ch_word in_use;

    // Members prefixed with "_" are nominally "private" Don't touch my privates!
    ch_word _add_next_index;
    ch_word _release_index;
    ch_word _use_next_index;

    ch_array_t* _array; //Actual cbuff storage
};

//Get access to an element at the front of the list
void* cbuff_peek_front(ch_cbuff_t* this);
//Pull an element from the front of the cbuff
int cbuff_pop_front(ch_cbuff_t* this);
//Use an item from the front of the queue
void* cbuff_use_front(ch_cbuff_t* this);
//Return an item to the front of the queue
void cbuff_unuse_front(ch_cbuff_t* this);
// Put an element at the back of the cbuff values
void* cbuff_push_back(ch_cbuff_t* this, void* value);
//Push back count elements the C cbuff to the back cbuff-list

void* cbuff_prev(ch_cbuff_t* this, void* curr);

void* cbuff_push_back_carray(ch_cbuff_t* this, void* ccbuff, ch_word* count);

//Free the resources associated with this cbuff, assumes that individual items have been freed
void cbuff_delete(ch_cbuff_t* this);

ch_cbuff_t* ch_cbuff_new(ch_word size, ch_word element_size);

#endif // CBUFF_H_
