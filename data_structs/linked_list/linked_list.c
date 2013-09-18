// CamIO 2: linked_list.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "linked_list.h"

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


//Return the element at a given offset, with bounds checking [WARN: This is slow in general]
static void** llist_off(ch_llist_t* this, ch_word idx)
{
    //Do this slow and stupid for now.
    idx = range_fix(this, idx);

    if(idx < 0){
        return NULL;
    }

    ch_llist_node_t* node = this->first;

    for(ch_word i = 0; i < idx; i++){
        if(node){ node = node->next; }
    }

    return node;

}

//Step forwards by one entry
static void** llist_next (ch_llist_t* this, void** ptr)
{
    if(*ptr){ node = node->next; }
}

//Step backwards by one entry
static void* llist_prev(ch_llist_t* this, void* ptr) {}
//Step forwards by amount
static void* llist_forward(ch_llist_t* this, void* ptr, ch_word amount) {}
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
