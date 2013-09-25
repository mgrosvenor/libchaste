/*
 * function_hash_map.h
 *
 *  Created on: Sep 23, 2013
 *      Author: mgrosvenor
 */

#ifndef FUNCTION_HASH_MAP_H_
#define FUNCTION_HASH_MAP_H_

#include "../../types/types.h"
#include "../linked_list/linked_list.h"
#include "../array/array.h"


struct ch_function_hash_map_t;
typedef struct ch_function_hash_map_t ch_function_hash_map;

typedef struct {
    ch_llist_t* list;
    ch_word offset;
    ch_word key_size;
    ch_word key_int; 	    //For keys less than or equal to 8bytes, just assign them
    void* key_ptr;			//For keys that are longer, and not static, alloc memory and copy them here
    void* key_ptr_unsafe; 	//For keys that are in mapped memory for the life of the program, just keep the pointer here
} ch_function_hash_map_node;

typedef struct {
    //These state variables are private
    ch_function_hash_map_node* _node;
    ch_llist_it item;

    //This is public
    void* key;
    ch_word key_size;
    void* value;
} ch_function_hash_map_it;


struct ch_function_hash_map_t{
    ch_word count;  //Return the actual number of elements in the function_hash_map

    // Members prefixed with "_" are nominally "private" Don't touch my privates!
   ch_word (*_cmp)(void* lhs, void* rhs); // Comparator function for find and sort operations
   ch_array_t* _backing_array;
   ch_word _element_size;
   ch_word (*_func)(ch_word value, void*);
};



//Return the element at a given offset, with bounds checking
ch_function_hash_map_it function_hash_map_off(ch_function_hash_map* this, ch_word idx);

//Get the first entry
ch_function_hash_map_it function_hash_map_first(ch_function_hash_map* this);
//Get the last entry
ch_function_hash_map_it function_hash_map_last(ch_function_hash_map* this);
//Get the end
ch_function_hash_map_it function_hash_map_end(ch_function_hash_map* this);

//Step forwards by one entry
void function_hash_map_next (ch_function_hash_map* this, ch_function_hash_map_it* it);
//Step backwards by one entry
void function_hash_map_prev(ch_function_hash_map* this, ch_function_hash_map_it*);
//Step forwards by amount
void function_hash_map_forward(ch_function_hash_map* this, ch_function_hash_map_it* it, ch_word amount);
//Step backwards by amount
void function_hash_map_back(ch_function_hash_map* this, ch_function_hash_map_it* it, ch_word amount);

// Put an element into the table,
ch_function_hash_map_it function_hash_map_push_unsafe_ptr(ch_function_hash_map* this,  void* key, ch_word key_size, void* value);
ch_function_hash_map_it function_hash_map_push(ch_function_hash_map* this,  void* key, ch_word key_size, void* value);
//Remove the given ptr
ch_function_hash_map_it function_hash_map_remove(ch_function_hash_map* this, ch_function_hash_map_it* itr);

//Free the resources associated with this function_hash_map, assumes that individual items have been freed
void function_hash_map_delete(ch_function_hash_map* this);

//Push back count elements the C function_hash_map to the back function_hash_map-list
ch_function_hash_map_it function_hash_map_push_back_carray(ch_function_hash_map* this, const void* keys, ch_word* key_sizes, const void* carray, ch_word count);

//Check for equality
ch_word function_hash_map_eq(ch_function_hash_map* this, ch_function_hash_map* that);

//Return the value associated with key using the comparator function
ch_function_hash_map_it function_hash_map_get_first(ch_function_hash_map* this, void* key, ch_word key_size);

//Get the next value with the given key
ch_function_hash_map_it function_hash_map_get_next(ch_function_hash_map_it it);

//Find the key of the given value
ch_function_hash_map_it function_hash_map_find(ch_function_hash_map* this, ch_function_hash_map_it* begin, ch_function_hash_map_it* end, void* value);

ch_function_hash_map* ch_function_hash_map_new( ch_word size, ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs) );

#endif // FUNCTION_HASH_MAP_H_

