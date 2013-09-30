/*
 * vector.h
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "../../types/types.h"
#include "../array/array.h"

struct ch_vector;
typedef struct ch_vector ch_vector_t;


struct ch_vector{
    ch_word size;  //Return the max number number of elements in the vector list
    ch_word count;  //Return the actual number of elements in the vector

    void* first; //Pointer to the fist valid entry list. Not valid if first == end
    void* last; //Pointer to the last valid element in the list. Not valid if last == end
    void* end; //Pointer to the one element beyond the end of the valid elements in list. Do not dereference!

    // Members prefixed with "_" are nominally "private" Don't touch my privates!
   ch_word (*_cmp)(void* lhs, void* rhs); // Comparator function for find and sort operations
   ch_array_t* _array; //Actual vector storage
   ch_word _array_count; //NUmber of elements currently in the vector
};


//Resize the vector
void vector_resize(ch_vector_t* this, ch_word new_size);

//Return the element at a given offset, with bounds checking
void* vector_off(ch_vector_t* this, ch_word idx);

//Step forwards by one entry
void* vector_next (ch_vector_t* this, void* ptr);
//Step backwards by one entry
void* vector_prev(ch_vector_t* this, void* ptr);
//Step forwards by amount
void* vector_forward(ch_vector_t* this, void* ptr, ch_word amount);
//Step backwards by amount
void* vector_back(ch_vector_t* this, void* ptr, ch_word amount);

// Put an element at the front of the vector list values, [WARN: In general this is very expensive for an vector]
void* vector_push_front(ch_vector_t* this, void* value);
// Push an element off the front of the vector list values, [WARN: In general this is very expensive for an vector]
void vector_pop_front(ch_vector_t* this);
// Put an element at the back of the vector values
void* vector_push_back(ch_vector_t* this, void* value);
// Push an element at the back of the vector values
void vector_pop_back(ch_vector_t* this);

// Insert an element after the element given by ptr
void* vector_insert_after(ch_vector_t* this, void* ptr, void* value);
// Insert an element before the element giver by ptr [WARN: In general this is very expensive for an vector]
void* vector_insert_before(ch_vector_t* this, void* ptr, void* value);
//Remove the given ptr [WARN: In general this is very expensive]
void* vector_remove(ch_vector_t* this, void* ptr);
//Pop everything out of the vector
void vector_clear(ch_vector_t* this);

//Free the resources associated with this vector, assumes that individual items have been freed
void vector_delete(ch_vector_t* this);

//Push back count elements the C vector to the back vector-list
void* vector_push_back_carray(ch_vector_t* this, void* cvector, ch_word count);

//Check for equality
ch_word vector_eq(ch_vector_t* this, ch_vector_t* that);
//find the given value using the comparator function
void* vector_find(ch_vector_t* this, void* begin, void* end, void* value);
//sort into order given the comparator function
void vector_sort(ch_vector_t* this);

ch_vector_t* ch_vector_new(ch_word size, ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs) );

#endif // VECTOR_H_
