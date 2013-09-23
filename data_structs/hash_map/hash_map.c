/*
 * hash_map.c
 *
 *  Created on: Sep 23, 2013
 *      Author: mgrosvenor
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../array/array.h"
#include "../linked_list/linked_list.h"
#include "hash_map.h"
#include "spooky_hash.h"
#include "../../utils/util.h"



static u64 hash(const void* key, ch_word key_size)
{
    return spooky_Hash64(key,key_size,0xFEEDBEEFCAFEB00BULL);
}


static ch_word hash_cmp(ch_hash_map_node* lhs, ch_hash_map_node* rhs)
{
    if(lhs->key_size < rhs->key_size){
        return -1;
    }

    if(lhs->key_size > rhs->key_size){
        return -1;
    }

    return strncmp(lhs->key, rhs->key, MIN(lhs->key_size, rhs->key_size));
}

//Return the value associated with key using the comparator function
ch_hash_map_it hash_map_get(ch_hash_map* this, void* key, ch_word key_size)
{

    ch_hash_map_it result = { 0 };

    ch_word idx = hash(key,key_size) % this->_backing_array->size;
    ch_llist_t* items  = array_off(this->_backing_array,idx);
    ch_llist_it first = llist_first(items);
    ch_llist_it end   = llist_end(items);
    ch_hash_map_node target = { .key = key, .key_size = key_size};
    ch_llist_it hash_map_node = llist_find(items,&first,&end,&target);

    if(!hash_map_node.value){
        return result;
    }

    result._node = (ch_hash_map_node*)hash_map_node.value;
    result.value = &(result._node->key) + 1;
    result.key =  result._node->key;
    result.key_size =  result._node->key_size;

    return result;
}

//Find the key of the given value
ch_hash_map_it hash_map_find(ch_hash_map* this, ch_hash_map_it* begin, ch_hash_map_it* end, void* value);


//Get the first entry
ch_hash_map_it hash_map_first(ch_hash_map* this);
//Get the last entry
ch_hash_map_it hash_map_last(ch_hash_map* this);
//Get the end
ch_hash_map_it hash_map_end(ch_hash_map* this);

//Step forwards by one entry
void hash_map_next (ch_hash_map* this, ch_hash_map_it* it);
//Step backwards by one entry
void hash_map_prev(ch_hash_map* this, ch_hash_map_it*);
//Step forwards by amount
void hash_map_forward(ch_hash_map* this, ch_hash_map_it* it, ch_word amount);
//Step backwards by amount
void hash_map_back(ch_hash_map* this, ch_hash_map_it* it, ch_word amount);

// Put an element into the table,
ch_hash_map_it hash_map_push(ch_hash_map* this, const void* key, ch_word key_size, const void* value)
{

    ch_hash_map_it result = { 0 };

    ch_word idx = hash(key,key_size) % this->_backing_array->size;
    ch_llist_t* items  = array_off(this->_backing_array,idx);

    //    ch_llist_t* list;
    //ch_word offset;
    //void* key;
    //ch_word key_size;

    ch_hash_map_node node  = { .list = items, .offset = idx, .key =
    ch_llist_it new_node = llist_push_back(items,&node);


    new_node



    llist_push_back(items,)

    if(!hash_map_node.value){
        return result;
    }

    result._node = (ch_hash_map_node*)hash_map_node.value;
    result.value = &(result._node->key) + 1;
    result.key =  result._node->key;
    result.key_size =  result._node->key_size;

    return result;
}


//Remove the given ptr
ch_hash_map_it hash_map_remove(ch_hash_map* this, ch_hash_map_it* itr);

//Free the resources associated with this hash_map, assumes that individual items have been freed
void hash_map_delete(ch_hash_map* this);

//Push back count elements the C hash_map to the back hash_map-list
ch_hash_map_it hash_map_push_back_carray(ch_hash_map* this, const void* keys, ch_word* key_sizes, const void* carray, ch_word count);

//Check for equality
ch_word hash_map_eq(ch_hash_map* this, ch_hash_map* that);

ch_hash_map* ch_hash_map_new( ch_word size, ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs) )
{
    if(element_size <= 0){
         printf("Error: invalid element size (<=0), must have *some* data\n");
         return NULL;
    }

    ch_hash_map* result = (ch_hash_map*)malloc(sizeof(ch_hash_map));
    if(!result){
        printf("Could not allocate memory for new hash_map structure. Giving up\n");
        return NULL;
    }

    result->count         = 0;
    result->_cmp          = (cmp_void_f)cmp;
    result->_element_size = element_size;

    result->_backing_array = ch_array_new(size, sizeof(ch_llist_t), NULL);
    for(ch_llist_t* it = result->_backing_array->first; result != NULL; array_next(result->_backing_array, it)){
        *it = *ch_llist_new(sizeof(ch_hash_map_node) + element_size, (cmp_void_f)hash_cmp);
    }

    return result;

}


