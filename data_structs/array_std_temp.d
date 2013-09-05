// CamIO 2: array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "array_std_int.h"

#include "../types/types.h"



/*Return the max number number of elements in the array*/
static ch_word size(ch_array_int_t* this)
{
    (void)this;
    return 0;
}

/*Return the actual number of elements in the array*/
static ch_word count(ch_array_int_t* this)
{
    (void)this;
    return 0;
}

/*Resize the array*/
static ch_word resize(ch_array_int_t* this, ch_word new_size)
{
    (void)this;
    (void)new_size;
    return 0;
}

/*Return the element at a given offset, with bounds checking*/
static int* off(ch_array_int_t* this, u64 idx)
{
    (void)this;
    (void)idx;
    return NULL;
}

/*Return an iterator at the given index*/
ch_array_int_iter_t* iter_at(ch_array_int_t* this, ch_word idx)
{
    (void)this;
    (void)idx;
    return NULL;
}

/*find the given value using the comparitor function*/
static ch_array_int_iter_t* find(ch_array_int_t* this, int value)
{
    (void)this;
    (void)value;
    return NULL;
}

/*sort into order given the comparitor function*/
static void sort(ch_array_int_t* this)
{
    (void)this;
}

/*sort into reverse order given the comparitor function*/
static void sort_reverse(ch_array_int_t* this)
{
    (void)this;
}


/* Put an element at the front of the array values, [WARN: In general this is very expensive for an array] */
static ch_array_int_iter_t* push_front(ch_array_int_t* this, int value)
{
    (void)this;
    return NULL;
}

/* Put an elemenet at the back of the arary values*/
static ch_array_int_iter_t* push_back(ch_array_int_t* this, int value)
{
    (void)this;
    return NULL;
}

/* Insert an element after the element given by iter*/
static ch_array_int_iter_t* insert_after(ch_array_int_t* this, ch_array_int_iter_t* iter, int value)
{
    (void)this;
    return NULL;
}

/* Insert an element befer the element giver by iter [WARN: In general this is very expensive for an array] */
static ch_array_int_iter_t* insert_before(ch_array_int_t* this, ch_array_int_iter_t* iter, int value)
{
    (void)this;
    (void)iter;
    (void)value;
    return NULL;
}

/*Set the given iter to the given value */
static ch_array_int_iter_t* assign(ch_array_int_t* this, ch_array_int_iter_t* iter, int value)
{
    (void)this;
    (void)iter;
    (void)value;
    return NULL;
}

/*Remove the given iter [WARN: In general this is very expensive] */
static ch_array_int_iter_t*(remove)(ch_array_int_t* this, ch_array_int_iter_t* iter)
{
    (void)this;
    (void)iter;
    return NULL;
}

/* Members prefixed with "_" are nominally "private" */
/* Comparator function for find and sort operations */
ch_word  (*_cmp)(int lhs, int rhs);
int* _array_backing; /*Actual array storage*/
ch_word _array_backing_size; /*Number of elements allocated in the given array*/
ch_word _array_backing_count; /*NUmber of elements currently in the array*/


ch_array_int_t* ch_array_int_new(ch_word size, int init_value, ch_word (*cmp)(int lhs, int rhs) )
{
    (void) size;
    (void)init_value;
    (void)cmp;
    return NULL;
}

