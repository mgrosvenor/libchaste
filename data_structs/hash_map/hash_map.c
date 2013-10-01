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
#include "../../hash_functions/spooky/spooky_hash.h"
#include "../../utils/util.h"


static u64 hash(const void* key, ch_word key_size)
{
    return spooky_Hash64(key,key_size,0xFEEDBEEFCAFEB00BULL);
}


static inline void* get_key(ch_hash_map_node* node)
{
	return node->key_ptr_unsafe ? node->key_ptr_unsafe : node->key_ptr ? node->key_ptr : &node->key_int;
}


static ch_word hash_cmp(ch_hash_map_node* lhs, ch_hash_map_node* rhs)
{
    if(lhs->key_size < rhs->key_size){
        return -1;
    }

    if(lhs->key_size > rhs->key_size){
        return -1;
    }

    //Fast path - simple ints
    if(!lhs->key_ptr_unsafe && !lhs->key_ptr && !rhs->key_ptr_unsafe && !rhs->key_ptr_unsafe){
    	return lhs->key_int == rhs->key_int ? 0 : lhs->key_int < rhs->key_int ? -1 : 1;
    }

    //Deal with mixed key locations
    const void* lhs_key = get_key(lhs);
    const void* rhs_key = get_key(rhs);

    //Fast paths
    switch(rhs->key_size){
        case 1: return  *(i8*)lhs_key  == *(i8*)rhs_key ? 0 :   * (i8*)lhs_key <  *(i8*)rhs_key ? -1 : 1;
        case 2: return  *(i16*)lhs_key == *(i16*)rhs_key ? 0 :  *(i16*)lhs_key < *(i16*)rhs_key ? -1 : 1;
        case 4: return  *(i32*)lhs_key == *(i32*)rhs_key ? 0 :  *(i32*)lhs_key < *(i32*)rhs_key ? -1 : 1;
        case 8: return  *(i64*)lhs_key == *(i64*)rhs_key ? 0 :  *(i64*)lhs_key < *(i64*)rhs_key ? -1 : 1;
    }

    //OK - do it the long way
    return strncmp(lhs_key, rhs_key, MIN(lhs->key_size, rhs->key_size));
}

static inline void assign_key(ch_hash_map_node* node, void* key, ch_word size, ch_bool unsafe)
{
	//Nice starting state
	node->key_size = size;
	node->key_int = 0;
	node->key_ptr = NULL;
	node->key_ptr_unsafe = NULL;

	//In unsafe mode, we keep just the pointer
	if(unsafe){
		node->key_ptr_unsafe = key;
		return;
	}

	//Fast paths - use simple assignment where possible
    switch(size){
        case 1: node->key_int = *(i8*)key;  return;
        case 2: node->key_int = *(i16*)key; return;
        case 4: node->key_int = *(i32*)key; return;
        case 8: node->key_int = *(i64*)key; return;
    }

    //Assign to local storage
    if(size < 8){
    	memcpy(&node->key_int, key, size);
    	return;
    }

    //OK - do it the long way
    node->key_ptr = malloc(size);
    memcpy(node->key_ptr, key, size);
}



//Return the value associated with key using the comparator function
ch_hash_map_it hash_map_get_first(ch_hash_map* this, void* key, ch_word key_size)
{

    ch_hash_map_it result = { 0 };

    ch_word idx = hash(key,key_size) % this->_backing_array->size;
    ch_llist_t* items  = array_off(this->_backing_array,idx);
    ch_llist_it first = llist_first(items);
    ch_llist_it end   = llist_end(items);
    ch_hash_map_node target = { 0 };
    assign_key(&target,key, key_size, true);//Use unsafe mode here, since this node is temporary for the life of the call
    ch_llist_it hash_map_node = llist_find(items,&first,&end,&target);

    if(!hash_map_node.value){
        return result;
    }

    result._node = (ch_hash_map_node*)hash_map_node.value;
    result.item = hash_map_node;
    result.value = ((u8*)hash_map_node.value) + sizeof(ch_hash_map_node);
    result.key =  get_key(result._node);
    result.key_size =  result._node->key_size;

    return result;
}

//Return the value associated with key using the comparator function
ch_hash_map_it hash_map_get_next(ch_hash_map_it it)
{

    ch_hash_map_it result = { 0 };

    if(!it._node){
    	return result;
    }

    ch_llist_t* items  = it._node->list;

    llist_next(items,&it.item);
    ch_llist_it first = it.item;
    ch_llist_it end   = llist_end(items);
    ch_hash_map_node target = { 0 };
    assign_key(&target,it.key, it.key_size, true);//Use unsafe mode here, since this node is temporary for the life of the call

    ch_llist_it hash_map_node = llist_find(items,&first,&end,&target);

    if(!hash_map_node.value){
        return result;
    }

    result._node = (ch_hash_map_node*)hash_map_node.value;
    result.item = hash_map_node;
    result.value = ((u8*)hash_map_node.value) + sizeof(ch_hash_map_node);
    result.key =  get_key(result._node);
    result.key_size =  result._node->key_size;

    return result;
}



////Find the key of the given value
//ch_hash_map_it hash_map_find(ch_hash_map* this, ch_hash_map_it* begin, ch_hash_map_it* end, void* value);
//
//
////Get the first entry
//ch_hash_map_it hash_map_first(ch_hash_map* this);
////Get the last entry
//ch_hash_map_it hash_map_last(ch_hash_map* this);
////Get the end
//ch_hash_map_it hash_map_end(ch_hash_map* this);
//
////Step forwards by one entry
//void hash_map_next (ch_hash_map* this, ch_hash_map_it* it);
////Step backwards by one entry
//void hash_map_prev(ch_hash_map* this, ch_hash_map_it*);
////Step forwards by amount
//void hash_map_forward(ch_hash_map* this, ch_hash_map_it* it, ch_word amount);
////Step backwards by amount
//void hash_map_back(ch_hash_map* this, ch_hash_map_it* it, ch_word amount);

// Put an element into the hash map. Unsafe assumes that the key is a pointer only, which is faster but assumes that storage doesn't go away.
static ch_hash_map_it _hash_map_push(ch_hash_map* this,  void* key, ch_word key_size, void* value, ch_bool unsafe)
{

    ch_hash_map_it result = { 0 };

    ch_word idx = hash(key,key_size) % this->_backing_array->size;
    ch_llist_t* items  = array_off(this->_backing_array,idx);

    ch_hash_map_node node  = { .list = items, .offset = idx};
    assign_key(&node,key, key_size, unsafe);

    ch_llist_it new_node   = llist_push_back(items,&node);
    memcpy( ((u8*)new_node.value) + sizeof(ch_hash_map_node), value, this->_element_size);


    result._node    = (ch_hash_map_node*)new_node.value;
    result.item     = new_node;

    result.value = ((u8*)new_node.value) + sizeof(ch_hash_map_node);
    result.key =  get_key(result._node);
    result.key_size =  result._node->key_size;
    this->count++;

    return result;
}


ch_hash_map_it hash_map_push(ch_hash_map* this,  void* key, ch_word key_size, void* value)
{
	return  _hash_map_push(this, key, key_size, value, false);
}

ch_hash_map_it hash_map_push_unsafe_ptr(ch_hash_map* this,  void* key, ch_word key_size, void* value)
{
	return  _hash_map_push(this, key, key_size, value, true);
}


ch_hash_map_it hash_map_first(ch_hash_map* this)
{
    ch_hash_map_it result = { 0 };
    ch_llist_t* node_list;
    ch_llist_it node_list_it;
    ch_hash_map_node * node = NULL;


    //Get the first offset that has a node in it
    for(ch_word i = 0; i < this->_backing_array->size; i++){
        node_list = (ch_llist_t*)array_off(this->_backing_array, i);

        if(!node_list){
            continue;
        }

        node_list_it = llist_first(node_list);
        if(node_list_it.value){
            node = node_list_it.value;
            break;
        }

    }

    //Nothing found, the hasmap must be empty
    if(!node){
        return result;
    }



    result._node = node;
    result.item = node_list_it;
    result.value =((u8*)node_list_it.value) + sizeof(ch_hash_map_node);
    result.key =  get_key(result._node);
    result.key_size =  result._node->key_size;

    return result;
}

////Get the last entry
//ch_hash_map_it hash_map_last(ch_hash_map* this);
////Get the end

ch_hash_map_it hash_map_end(ch_hash_map* this)
{
    (void)this;
    ch_hash_map_it result = { 0 };
    return result;
}


////Step forwards by one entry
void hash_map_next (ch_hash_map* this, ch_hash_map_it* it)
{
    ch_hash_map_it result = { 0 };

    ch_llist_t* node_list = NULL;
    ch_llist_it node_list_it = it->item;
    ch_hash_map_node * node = it->_node;
    ch_word idx = node->offset;
    node = NULL;

    idx++;
    if(idx >= this->_backing_array->size){
        *it = result;
        return;
    }

    node_list = (ch_llist_t*)array_off(this->_backing_array, idx);
    llist_next(node_list, &node_list_it);

    for(;idx < this->_backing_array->size;idx++){
        if(node_list_it.value){
            node = node_list_it.value;
            break;
        }

        node_list = (ch_llist_t*)array_off(this->_backing_array, idx);
        node_list_it = llist_first(node_list);

    }

    //Nothing found, the hasmap must be empty
    if(!node){
        *it = result;
        return;
    }

    result._node = node;
    result.item = node_list_it;
    result.value =((u8*)node_list_it.value) + sizeof(ch_hash_map_node);
    result.key =  get_key(result._node);
    result.key_size =  result._node->key_size;
    *it = result;

}


//Remove the given ptr
//ch_hash_map_it hash_map_remove(ch_hash_map* this, ch_hash_map_it* itr);


//Push back count elements the C hash_map to the back hash_map-list
//ch_hash_map_it hash_map_push_back_carray(ch_hash_map* this, const void* keys, ch_word* key_sizes, const void* carray, ch_word count);

//Check for equality
//ch_word hash_map_eq(ch_hash_map* this, ch_hash_map* that);

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

    for(ch_llist_t* it = result->_backing_array->first; it != result->_backing_array->end; it = array_next(result->_backing_array, it)){
        ch_llist_init(it, sizeof(ch_hash_map_node) + element_size, (cmp_void_f)hash_cmp);
    }

    return result;

}



//Free the resources associated with this hash_map, assumes that individual items have been freed
void hash_map_delete(ch_hash_map* this)
{
    if(!this){
        return;
    }

    //Free up all of the lists
    for(ch_llist_t* it = this->_backing_array->first; it != this->_backing_array->end; it = array_next(this->_backing_array, it)){
        //Can't use free here because the list object is statically allocated
        llist_pop_all(it); //Free all the list nodes
    }

    //Free up the array
    array_delete(this->_backing_array);

    free(this);
}

