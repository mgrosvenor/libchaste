// CamIO 2: linked_list.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "linked_list.h"


#define NODE_DATAP(node) ( (void*)((u8*)node  + sizeof(ch_llist_node_t)) )
#define NODE_DATA(node)  ( (void*)((u8*)&node + sizeof(ch_llist_node_t)) )

//Allocate an object using whatever mechanism we want
static ch_llist_node_t* alloc_ch_llist_node_obj(ch_llist_t* this){
    ch_llist_node_t* result = (ch_llist_node_t*)malloc(sizeof(ch_llist_node_t) + this->_element_size );
    return result;
}

static void free_ch_llist_node_obj(ch_llist_t* this, ch_llist_node_t* lhs){
    (void)this;
    free(lhs);
}


//Get the first entry
ch_llist_it llist_first(ch_llist_t* this)
{
    ch_llist_it result = {0};
    result._node = this->_first;
    result.value = result._node ? NODE_DATAP(result._node) : NULL;
    return result;
}

//Get the last entry
ch_llist_it llist_last(ch_llist_t* this)
{
    ch_llist_it result = {0};
    result._node = this->_last;
    result.value = result._node ? NODE_DATAP(result._node) : NULL;
    return result;

}

//Get the end
ch_llist_it llist_end(ch_llist_t* this)
{
    (void)this;
    ch_llist_it result = {0};
    return result;
}


//Step through the list in any direction by any amount
static ch_llist_it step_through_list(ch_llist_t*this, ch_llist_node_t* start, ch_word amount)
{

    (void)this;
    ch_llist_it result = { 0 };
    ch_llist_node_t* node = start;

    const ch_word dir = amount > 0 ? 1 : -1;

    for(ch_word i = 0; i < abs(amount); i++){
        if(node){
            node = dir ? node->next : node->prev;
        }
    }

    result._node = node;
    result.value = result._node ? NODE_DATAP(result._node) : NULL;

    return result;

}


//Return the element at a given offset, with bounds checking [WARN: This is slow in general]
ch_llist_it llist_off(ch_llist_t* this, ch_word idx)
{
    ch_llist_node_t* start = idx < 0 ? this->_last : this->_first;
    idx = idx < 0 ? idx + 1 : idx; //Correct for the change above

    return step_through_list(this, start, idx );
}


//Step forwards by amount
void llist_forward(ch_llist_t* this, ch_llist_it* itr_inout, ch_word amount)
{
    *itr_inout = step_through_list(this, itr_inout->_node, amount);
}

//Step backwards by amount
void llist_back(ch_llist_t* this, ch_llist_it* itr_inout, ch_word amount)
{
    *itr_inout = step_through_list(this, itr_inout->_node, -1 * amount);
}


//Step forwards by one entry
void llist_next (ch_llist_t* this, ch_llist_it* it_inout)
{
    *it_inout = step_through_list(this, it_inout ->_node, 1);
}

//Step backwards by one entry
void llist_prev(ch_llist_t* this, ch_llist_it* it_inout)
{
    *it_inout = step_through_list(this, it_inout->_node, -1);
}


// Put an element at the front of the llist list values,
ch_llist_it llist_push_front(ch_llist_t* this, const void* value)
{
    ch_llist_it result = { 0 };
    ch_llist_node_t* node = alloc_ch_llist_node_obj(this);

    if(!node){
        return result;
    }

    memcpy(NODE_DATAP(node), value, this->_element_size);
    node->prev = NULL;
    node->next = this->_first;

    if(this->_first){
        this->_first->prev = node;
    }

    this->_first = node;

    if(this->_last == NULL){
        this->_last = this->_first;
    }
    else if(this->_last == this->_first){
        this->_last = node->next;
    }

    this->count++;
    result._node = node;
    result.value = result._node ? NODE_DATAP(result._node) : NULL;

    return result;

}


// Put an element at the back of the llist values
ch_llist_it llist_push_back(ch_llist_t* this, const void* value)
{

    ch_llist_it result = { 0 };
    ch_llist_node_t* node = alloc_ch_llist_node_obj(this);

    if(!node){
        return result;
    }

    memcpy( NODE_DATAP(node), value, this->_element_size);
    node->next = NULL;
    node->prev = this->_last;

    if(this->_last){
        this->_last->next = node;
    }

    this->_last = node;

    if(this->_first == NULL){
        this->_first = this->_last;
    }
    else if(this->_first == this->_last){
        this->_first = node->prev;
    }


    this->count++;
    result._node = node;
    result.value = result._node ? NODE_DATAP(result._node) : NULL;
    return result;
}



// Insert an element after the element given by ptr
ch_llist_it* llist_insert_after(ch_llist_t* this, ch_llist_it* itr, const void* value)
{
    if(!itr){
        return NULL;
    }

    ch_llist_it result = { 0 };
    ch_llist_node_t* node = alloc_ch_llist_node_obj(this);

    if(!node){
        *itr = result;
        return itr;
    }

    memcpy(NODE_DATAP(node), value, this->_element_size);

    //Rearrange the pointers.
    if(itr->_node->next){
        node->next = itr->_node->next;
        itr->_node->next->prev = node;
    }
    else{
        this->_last = node;
    }

    node->prev       = itr->_node;
    itr->_node->next = node;

    result._node = node;
    result.value = NODE_DATAP(node);

    this->count++;
    *itr = result;
    return itr;

}


// Insert an element before the element giver by ptr
ch_llist_it* llist_insert_before(ch_llist_t* this, ch_llist_it* itr, const void* value)
{
    if(!itr){
        return NULL;
    }

    ch_llist_it result = { 0 };
    ch_llist_node_t* node = alloc_ch_llist_node_obj(this);

    if(!node){
        *itr = result;
        return itr;
    }

    memcpy(NODE_DATAP(node), value, this->_element_size);

    //Rearrange the pointers.
    if(itr->_node->prev){
        node->prev = itr->_node->prev;
        itr->_node->prev->next = node;
    }
    else{
        this->_first = node;
    }
    node->next       = itr->_node;
    itr->_node->prev = node;

    result._node = node;
    result.value = NODE_DATAP(node);

    this->count++;
    *itr = result;

    return itr;

}

//Remove the item given by the iterator
ch_llist_it llist_remove_it(ch_llist_t* this, ch_llist_it* itr)
{
    ch_llist_it result = { 0 };
    if(!itr){
        return result;
    }

    if(!itr->_node){
        return result;
    }

    if(!itr->_node->prev){
        this->_first = itr->_node->next;
    }
    else{
        itr->_node->prev->next = itr->_node->next;
    }

    if(!itr->_node->next){
        this->_last = itr->_node->prev;
    }
    else{
        itr->_node->next->prev = itr->_node->prev;

        result._node = itr->_node->next;
        result.value = result._node ? NODE_DATAP(result._node) : NULL;
    }

    free_ch_llist_node_obj(this, itr->_node);
    this->count--;

    return result;
}

//Remove the item given by the iterator
ch_llist_it llist_remove_all(ch_llist_t* this,  void* value)
{
    ch_llist_it result = { 0 };
    ch_llist_it found = llist_find_first(this,value);
    for(int i = 0; found.value; i++){
        result = llist_remove_it(this,&found);
        found = llist_find_next(this,&result,value);
    }

    return result;
}




// Push an element at the back of the llist values
ch_llist_it llist_pop_back(ch_llist_t* this)
{
    ch_llist_it it = llist_last(this);
    return llist_remove_it(this,&it);
}

// Push an element off the front of the llist list values,
ch_llist_it llist_pop_front(ch_llist_t* this)
{
    ch_llist_it it = llist_first(this);
    return llist_remove_it(this,&it);
}


//Get rid of everything
void llist_pop_all(ch_llist_t* this)
{
    ch_llist_node_t* node = this->_first;

    while(node){
        ch_llist_node_t* tmp = node;
        node = node->next;
        free_ch_llist_node_obj(this, tmp);
    }
}



//Free the resources associated with this llist, assumes that individual items have been freed
void llist_delete(ch_llist_t* this)
{
    if(!this){
        return;
    }

    llist_pop_all(this);

    free(this);

}



//Push back count elements the C llist to the back llist-list
ch_llist_it llist_push_back_carray(ch_llist_t* this, const void* carray, ch_word count)
{
    ch_llist_it result;

    const ch_byte* ptr = carray;
    for(ch_word i =  0; i < count; i++){
        result = llist_push_back(this,ptr);
        ptr += this->_element_size;
    }

    return result;
}

//Check for equality
ch_word llist_eq(ch_llist_t* this, ch_llist_t* that)
{
    if(!this && !that){
        return 1;
    }

    if(!this){
        return 0;
    }

    if(!that){
        return 0;
    }

    if(this->count != that->count){
        return 0;
    }

    if(this->count == 0 && that->count == 0){
        return 1;
    }

    ch_llist_it it1 = llist_first(this);
    ch_llist_it it2 = llist_first(that);
    for( ; it1.value && it2.value; llist_next(this,&it1), llist_next(that,&it2))
    {
        if(this->_cmp(it1.value, it2.value)){
            return 0;
        }
    }

    return 1;
}

//find the given value using the comparator function
ch_llist_it llist_find(ch_llist_t* this, ch_llist_it* begin, ch_llist_it* end, void* value)
{
    ch_llist_it result = { 0 };

    if(begin == NULL || end == NULL){
        return result;
    }

    ch_llist_it it = *begin;
    for(; it._node && it._node != end->_node; llist_next(this,&it) ){
        if(this->_cmp(it.value, value) == 0){
            return it;
        }
    }

    return result;
}

ch_llist_it llist_find_first(ch_llist_t* this, void* value)
{
    ch_llist_it first  = llist_first(this);
    ch_llist_it last   = llist_end(this);
    ch_llist_it result = llist_find(this,&first,&last,value);
    return result;
}

ch_llist_it llist_find_next(ch_llist_t* this,  ch_llist_it* begin, void* value)
{
    ch_llist_it last   = llist_end(this);
    ch_llist_it result = llist_find(this,begin,&last,value);
    return result;
}


//sort into order given the comparator function
void llist_sort(ch_llist_t* this)
{
    (void)this;
    printf("%s:%s%u Not (yet  )implemented!\n", __FUNCTION__, __FILE__, __LINE__ );
    exit(1);
}

ch_llist_t* ch_llist_new( ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs) )
{

     ch_llist_t* result = (ch_llist_t*)calloc(1,sizeof(ch_llist_t));
    if(!result){
        printf("Could not allocate memory for new llist structure. Giving up\n");
        return NULL;
    }

    return ch_llist_init(result, element_size,cmp);

}

ch_llist_t* ch_llist_init( ch_llist_t* this, ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs) )
{

    if(element_size <= 0){
        printf("Error: invalid element size (<=0), must have *some* data\n");
        return NULL;
    }


    /*We have memory to play with, now do all the other assignments*/
    this->_cmp            = cmp;
    this->_element_size   = element_size;
    this->_first          = NULL;
    this->_last           = NULL;
    this->count           = 0;

    return this;

}


