// CamIO 2: linked_list.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "linked_list.h"


//Allocate an object using whatever mechanism we want
static ch_llist_node_t* alloc_ch_llist_node_obj(ch_llist_t* this){
    ch_llist_node_t* result = (ch_llist_node_t*)malloc(sizeof(ch_llist_node_t) + this->_element_size );
    return result;
}

static void free_ch_llist_node_obj(ch_llist_t* this, ch_llist_node_t* lhs){
    free(lhs);
}


//Get the first entry
static ch_llist_it llist_first(ch_llist_t* this)
{
    ch_llist_it result = {0};
    result._node = this->_first;
    result.value = result._node ? result._node->data : NULL;
    return result;
}

//Get the last entry
static ch_llist_it llist_last(ch_llist_t* this)
{
    ch_llist_it result = {0};
    result._node = this->_last;
    result.value = result._node ? result._node->data : NULL;
    return result;

}

//Get the end
static ch_llist_it llist_end(ch_llist_t* this)
{
    ch_llist_it result = {0};
    return result;
}


//Step through the list in any direction by any amount
static ch_llist_it step_through_list(ch_llist_t*this, ch_llist_it* start, ch_word amount)
{

    ch_llist_it result = { 0 };
    ch_llist_node_t* node = start;

    const ch_word dir = amount > 0 ? 1 : -1;

    for(ch_word i = 0; i < abs(amount); i++){
        if(node){
            node = dir ? node->next : node->prev;
        }
    }

    result._node = node;
    result.value = result._node ? result._node->data : NULL;

    return result;

}


//Return the element at a given offset, with bounds checking [WARN: This is slow in general]
static ch_llist_it llist_off(ch_llist_t* this, ch_word idx)
{
    ch_llist_it result;

    ch_llist_node_t* start = idx < 0 ? this->_last : this->_first;
    idx = idx < 0 ? idx + 1 : idx; //Correct for the change above

    return step_through_list(this, start, idx );
}


//Step forwards by amount
static ch_llist_it* llist_forward(ch_llist_t* this, ch_llist_it* itr, ch_word amount)
{
    return (*itr = step_through_list(this, this, amount) ) ;
}

//Step backwards by amount
static ch_llist_it* llist_back(ch_llist_t* this, ch_llist_it* itr, ch_word amount)
{
    return (*itr = step_through_list(this, this, -1 * amount) ) ;
}


//Step forwards by one entry
static ch_llist_it* llist_next (ch_llist_t* this, ch_llist_it* it)
{
    return (*it = step_through_list(this, this, 1) ) ;
}

//Step backwards by one entry
static ch_llist_it* llist_prev(ch_llist_t* this, ch_llist_it* it)
{
    return (*it = step_through_list(this, this, -1) ) ;
}


// Put an element at the front of the llist list values,
static ch_llist_it llist_push_front(ch_llist_t* this, const void* value)
{
    ch_llist_it result = { 0 };
    ch_llist_node_t* node = alloc_ch_llist_node_obj();

    if(!node){
        return result;
    }

    memcpy(node->data, value, this->_element_size);
    node->prev = NULL;
    node->next = this->_first;

    if(this->_first){
        this->_first->prev = node;
    }

    if(this->_last == NULL){
        this->_last = this->_first;
    }
    else if(this->_last == this->_first){
        this->_last = node->next;
    }

    this->_first = node;

    result._node = node;
    result.value = result._node ? result._node->data : NULL;

    this->count++;

}


// Put an element at the back of the llist values
static void* llist_push_back(ch_llist_t* this, const void* value)
{

    ch_llist_it result = { 0 };
    ch_llist_node_t* node = alloc_ch_llist_node_obj();

    if(!node){
        return result;
    }

    memcpy(node->data, value, this->_element_size);
    node->next = NULL;
    node->prev = this->_last;

    if(this->_last){
        this->_last->next = node;
    }

    if(this->_first == NULL){
        this->_first = this->_last;
    }
    else if(this->_first == this->_last){
        this->_first = node->prev;
    }

    this->_last = node;
    this->count++;
}



// Insert an element after the element given by ptr
static ch_llist_it* llist_insert_after(ch_llist_t* this, ch_llist_it* itr, const void* value)
{
    if(!itr){
        return NULL;
    }

    ch_llist_it result = { 0 };
    ch_llist_node_t* node = alloc_ch_llist_node_obj();

    if(!node){
        return result;
    }

    memcpy(node->data, value, this->_element_size);

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

    result->_node = node;
    result->value = node->data;

    this->count++;
    return result;

}


// Insert an element before the element giver by ptr
static ch_llist_it* llist_insert_before(ch_llist_t* this, ch_llist_it* itr, const void* value)
{
    if(!itr){
        return NULL;
    }

    ch_llist_it result = { 0 };
    ch_llist_node_t* node = alloc_ch_llist_node_obj();

    if(!node){
        return result;
    }

    memcpy(node->data, value, this->_element_size);

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

    result->_node = node;
    result->value = node->data;

    this->count++;
    return result;

}


//Remove the given ptr [WARN: In general this is very expensive]
static ch_llist_it llist_remove(ch_llist_t* this, ch_llist_it* itr)
{
    ch_llist_it result = { 0 };
    if(!itr){
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

        result->_node = itr->_node->next;
        result.value = result._node ? result._node->data : NULL;
    }

    free_ch_llist_node_obj(this, itr->_node);
    this->count--;

    return result;
}


// Push an element at the back of the llist values
static ch_llist_it llist_pop_back(ch_llist_t* this)
{
    return llist_remove(this,llist_last(this));
}

// Push an element off the front of the llist list values,
static ch_llist_it llist_pop_front(ch_llist_t* this)
{
    return llist_remove(this,llist_first(this));
}

//Free the resources associated with this llist, assumes that individual items have been freed
static void llist_delete(ch_llist_t* this)
{
    ch_llist_node_t* node = this->_first;

    while(node){
        free_ch_llist_node_obj(this, node);
        node = node->next;
    }

    free(this);

}



//Push back count elements the C llist to the back llist-list
static ch_llist_it llist_push_back_carray(ch_llist_t* this, const void* carray, ch_word count)
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
    if(this->count != that->count){
        return 0;
    }

    if(this->count == 0 && that->count == 0){
        return 1;
    }

    ch_llist_it it1 = llist_first(this);
    ch_llist_it it2 = llist_first(that);
    for( ; it1->value && it2->value; llist_next(this,&it1), llist_next(that,&it2))
    {
        if(!this->_cmp(it1->value, it2->value)){
            return 0;
        }
    }

    return 1;
}


//find the given value using the comparator function
static ch_llist_it llist_find(ch_llist_t* this, ch_llist_it* begin, ch_llist_it* end, void* value)
{
    ch_llist_it result = { 0 };

    for(ch_llist_it it = begin; it != end; llist_next(this,&it) ){
        if(this->_cmp(it->value, value) == 0){
            return it;
        }
    }

    return result;

}
//sort into order given the comparator function
static void llist_sort(ch_llist_t* this)
{
    printf("%s:%s%u Not (yet  )implemented!\n", __FUNCTION__, __FILE__, __LINE__ );
    exit(1);
}

ch_llist_t* ch_llist_new(ch_word size, ch_word element_size, ch_word(*cmp)(static void* lhs, void* rhs) )
{
    ch_llist_t* result = (ch_llist_t*)calloc(1,sizeof(ch_llist_t));
    if(!result){
        printf("Could not allocate memory for new llist structure. Giving up\n");
        return NULL;
    }


    /*We have memory to play with, now do all the other assignments*/
    result->_cmp            = cmp;
    result->_element_size   = element_size;
    result->_first          = NULL;
    result->_last           = NULL;
    result->count           = 0;

    result->first                   = llist_first;
    result->last                    = result->first;
    result->end                     = result->first;
    result->size                    = result->_array->size;
    result->count                   = result->_array_count;

    return result;

}
