// CamIO 2: linked_list.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "linked_list.h"




//Get the first entry
static ch_llist_it llist_first(ch_llist_t* this)
{

}

//Get the last entry
static ch_llist_it llist_last(ch_llist_t* this)
{

}

//Get the end
static ch_llist_it llist_end(ch_llist_t* this)
{

}


//Take an index an fix it so that negative indexs are legal
static inline ch_word range_fix(ch_llist_t* this, ch_word idx)
{
    idx = idx < 0 ? idx + this : idx;

    if(idx >= 0 && idx < this->count) {
        return idx;
    }

    printf("Index (%li) is out of the valid range [%li,%li]\n", idx, -1 * this->count, this->count - 1 );
    return -1;

}


static ch_list_it step_through_list(ch_llist_t*this, ch_llist_it* start, ch_word amount)
{

    ch_llist_it result = { 0 };
    ch_llist_node_t* node = start;

    const ch_word dir = amount > 0 ? 1 : -1;

    //Do this slow and stupid for now.
    for(ch_word i = 0; i < amount; i++){
        if(node){
            node = dir ? node->next : node->prev; }
    }

    result->_node = node;
    if(node){
        result->value = node->data;
    }
    else{
        result->value = NULL;
    }

    return result;

}



//Return the element at a given offset, with bounds checking [WARN: This is slow in general]
static ch_llist_it llist_off(ch_llist_t* this, ch_word idx)
{
    ch_llist_it result;


    idx = range_fix(this, idx);

    if(idx < 0){
        result->_node = NULL;
        result->value = NULL;
        return result;
    }

    step_through_list(this, result->` )
    return result;

}


//Step forwards by amount
static ch_llist_it* llist_forward(ch_llist_t* this, ch_llist_it* ptr, ch_word amount)
{

}


//Step forwards by one entry
static ch_llist_it* llist_next (ch_llist_t* this, ch_llist_it* it)
{

    ch_llist_it* node = NULL;
    if(it){
        node = it->_node->next;
    }

    it->_node = node;
    if(node){

    }
}

//Step backwards by one entry
static void* llist_prev(ch_llist_t* this, void* ptr) {}
//Step backwards by amount
static void* llist_back(ch_llist_t* this, void* ptr, ch_word amount) {}

// Put an element at the front of the llist list values,
static void* llist_push_front(ch_llist_t* this, void* value) {}
// Push an element off the front of the llist list values,
static void llist_pop_front(ch_llist_t* this) {}
// Put an element at the back of the llist values
static void* llist_push_back(ch_llist_t* this, void* value) {}
// Push an element at the back of the llist values
static void llist_pop_back(ch_llist_t* this) {}

// Insert an element after the element given by ptr
static void* llist_insert_after(ch_llist_t* this, void* ptr, void* value) {}
// Insert an element before the element giver by ptr
static void* llist_insert_before(ch_llist_t* this, void* ptr, void* value) {}
//Remove the given ptr [WARN: In general this is very expensive]
static void* llist_remove(ch_llist_t* this, void* ptr) {}
//Free the resources associated with this llist, assumes that individual items have been freed
static void llist_delete(ch_llist_t* this) {}

//Push back count elements the C llist to the back llist-list
static void* llist_push_back_carray(ch_llist_t* this, void* cllist, ch_word count) {}

//Check for equality
ch_word llist_eq(ch_llist_t* this, ch_llist_t* that);
//find the given value using the comparator function
static void* llist_find(ch_llist_t* this, void* begin, void* end, void* value) {}
//sort into order given the comparator function
static void llist_sort(ch_llist_t* this) {}

ch_llist_t* ch_llist_new(ch_word size, ch_word element_size, ch_word(*cmp)(static void* lhs, void* rhs) ) {}
