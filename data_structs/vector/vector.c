/*
 * vector.c
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../../utils/util.h"
#include "../../types/types.h"
#include "../array/array.h"
#include "vector.h"


static void* _vector_forward_unsafe(ch_vector_t* this, void* ptr, ch_word amount)
{
    return (ch_byte*)ptr + this->_array->_element_size * amount;
}

static void* _vector_back_unsafe(ch_vector_t* this, void* ptr, ch_word amount)
{
    return (ch_byte*)ptr - this->_array->_element_size * amount;
}



void* vector_forward(ch_vector_t* this, void* ptr, ch_word amount)
{
    if((ch_byte*)ptr + this->_array->_element_size * amount <= (ch_byte*) this->end){
        return (ch_byte*)ptr + this->_array->_element_size * amount;
    }

    printf("vector forward catch\n");

    return ptr;
}

void* vector_back(ch_vector_t* this, void* ptr, ch_word amount)
{
    if((ch_byte*)ptr - this->_array->_element_size * amount >= (ch_byte*)this->first){
        return (ch_byte*)ptr - this->_array->_element_size * amount;
    }

    printf("vector backward catch\n");

    return ptr;
}



void* vector_next(ch_vector_t* this, void* ptr)
{
    return vector_forward(this, ptr, 1);
}

void* vector_prev(ch_vector_t* this, void* ptr)
{
    return vector_back(this, ptr, 1);
}



void vector_resize(ch_vector_t* this, ch_word new_size)
{
    array_resize(this->_array,new_size);

    this->_array_count         = MIN(new_size, this->_array_count);
    this->count                = this->_array_count;
    this->size                 = new_size;

    if(new_size == 0){
        this->first          = NULL;
        this->last           = NULL;
        this->end            = NULL;
    }
    else{
        this->first            = this->_array->first;
        this->end              = _vector_forward_unsafe(this, this->first, this->_array_count);
        this->last             = _vector_back_unsafe(this, this->end, 1);
    }

}


/*Return the element at a given offset, with bounds checking*/
void* vector_off(ch_vector_t* this, ch_word idx)
{

    if(idx >= this->_array_count){
        printf("Index (%li) is out of the valid range [%li,%li]", idx, -1 * this->_array_count, this->_array_count - 1 );
        return NULL;
    }

    return array_off(this->_array,idx);

}



/*Check for equality between two vector lists*/
ch_word vector_eq(ch_vector_t* this, ch_vector_t* that)
{

    if(this->_array_count != that->_array_count){
        return 0;
    }

    void* i = this->first;
    void* j = that->first;
    for(; i < this->end && j < that->end; i = vector_next(this, i), j = vector_next(that, j)){
        if( this->_cmp(i,j) ){
            return 0;
        }
    }

    return 1;
}






/*find the given value using the comparitor function*/
void* vector_find(ch_vector_t* this, void* begin, void* end, void* value)
{
    return array_find(this->_array, begin, end, value);
}

/*sort into reverse order given the comparitor function*/
void vector_sort(ch_vector_t* this)
{
    qsort(this->first, this->count, this->_array->_element_size, (__compar_fn_t)this->_array->_cmp);
}


/* Insert an element before the element giver by ptr [WARN: In general this is very expensive for an vector] */
void* vector_insert_before(ch_vector_t* this, void* ptr, void* value)
{

    /*If the backing memory is full, grow the vector*/
    if(unlikely(this->_array_count == this->_array->size)){
        const ch_word ptr_idx = ptr ? (ch_byte*)ptr - (ch_byte*)this->_array->first: 0;
        const ch_word new_size = this->size ? this->size * 2 : 1;
        vector_resize(this,new_size);
        ptr = (ch_byte*)this->_array->first + ptr_idx;
    }

    if(unlikely(ptr < this->first)){
        printf("ptr supplied is out of range. Too small.\n");
        return NULL;
    }

    /* NB: It's ok to insert *before* last + 1. This essentially inserts at last which is the last item. */
    if(unlikely((ch_byte*)ptr > (ch_byte*)this->last + this->_array->_element_size)){
        printf("ptr supplied is out of range. Too big.\n");
        return NULL;
    }

    /* Optimise for the push_back case*/
    if(unlikely(this->_array_count  && ptr < this->end )){
        void* dst = _vector_forward_unsafe(this,ptr,1);
        const void* src = ptr;
        const ch_word amount =  ((ch_byte*)this->end - (ch_byte*)ptr );
        memmove( dst ,src, amount);
    }

    memcpy(ptr,value,this->_array->_element_size);

    if(unlikely(this->_array_count == 0)){
        this->first = this->_array->first;
        this->last  = this->first;
        this->end   = _vector_forward_unsafe(this, this->last,1);
    }
    else{
        this->last   = _vector_forward_unsafe(this, this->last,1);
        this->end   = _vector_forward_unsafe(this, this->end,1);
    }

    this->_array_count++;
    this->count++;

    return ptr;
}

/* Insert an element after the element given by ptr* [WARN: In general this is very expensive for an vector] */
void* vector_insert_after(ch_vector_t* this, void* ptr, void* value)
{
    /*Inserting after is the equivalent to inserting before, the value after the current */
    ptr = this->_array->size == 0 ? NULL : _vector_forward_unsafe(this,ptr, 1);
    return vector_insert_before(this,ptr,value);
}



/* Put an element at the front of the vector values, [WARN: In general this is very expensive for an vector] */
void* vector_push_front(ch_vector_t* this, void* value)
{
    /* Pushing onto the front is equivalent to inserting at the head */
    void* ptr = this->_array->size == 0 ? NULL : this->first;
    return vector_insert_before(this, ptr, value);
}

/* Put an element at the back of the arary values*/
void* vector_push_back(ch_vector_t* this, void* value)
{
    /* Pushing onto the end is equivalent to inserting at the tail */
    void* ptr = this->_array->size == 0 ? NULL : this->end;
    return vector_insert_before(this, ptr, value);
}


/*Remove the given ptr [WARN: In general this is very expensive], return an to the next item in the list */
void* vector_remove(ch_vector_t* this, void* ptr)
{

    if(unlikely(this->first == this->end)){
        printf("Array list is emptyn");
        return NULL;
    }

    if(unlikely(ptr < this->_array->first)){
        printf("ptr supplied is out of range. Too small.\n");
        return NULL;
    }

    if(unlikely(ptr > this->last)){
        printf("ptr supplied is out of range. Too big.\n");
        return NULL;
    }

    /*Slow (but likely), keep the fast path fast*/
    if(unlikely(ptr != this->last)){
/*
        printf("ptr:%p[%li], first:%p[%li], last:%p[%li], end:%p:[%li] (%lu)\n",
                (void*)ptr, ptr - this->first,
                (void*)this->first, this->first - this->first,
                (void*)this->last, this->last - this->first,
                (void*)this->end, this->end - this->first,
                (this->last) - (ptr));
*/
        memmove(ptr, _vector_forward_unsafe(this,ptr, 1), (ch_byte*)this->last - (ch_byte*)ptr );
    }

    this->_array_count--;
    this->count--;


    if(unlikely(this->_array_count == 0)){
        this->first = this->_array->first;
        this->last = this->first;
        this->end  = this->last;
        ptr = NULL;
    }
    else{
        this->last = _vector_back_unsafe(this, this->last, 1);
        this->end = _vector_back_unsafe(this, this->end, 1);
    }

    return ptr;
}

/* Put an element at the front of the vector values, [WARN: In general this is very expensive for an vector] */
void vector_pop_front(ch_vector_t* this)
{
    vector_remove(this,this->first);
}

/* Put an element at the front of the vector values, [WARN: In general this is very expensive for an vector] */
void vector_pop_back(ch_vector_t* this)
{
    vector_remove(this,this->last);
}

void vector_delete(ch_vector_t* this)
{
    if(this->_array){
        array_delete(this->_array);
    }

    free(this);
}


/*Assign at most size elements from the C vector*/
void* vector_push_back_carray(ch_vector_t* this, void* carray, ch_word count)
{
    if(this->size - this->_array_count < count){
        const ch_word new_size = this->size ? this->size * 2 : next_pow2(count);
        vector_resize(this,new_size);
    }

    memcpy(this->end, carray, count * this->_array->_element_size );
    this->_array_count += count;
    this->count = this->_array_count;
    this->last = _vector_forward_unsafe(this,this->last,count);
    this->end = _vector_forward_unsafe(this, this->last, 1);

    return this->last;
}


ch_vector_t* ch_vector_new(ch_word size, ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs) )
{

    ch_vector_t* result = (ch_vector_t*)calloc(1,sizeof(ch_vector_t));
    if(!result){
        printf("Could not allocate memory for new vector structure. Giving up\n");
        return NULL;
    }

    result->_array       = ch_array_new(size, element_size, cmp);

    /*We have memory to play with, now do all the other assignments*/
    result->_array_count            = 0;
    result->_cmp                    = cmp;
    result->first                   = result->_array->first;
    result->last                    = result->first;
    result->end                     = result->first;
    result->size                    = result->_array->size;
    result->count                   = result->_array_count;

    return result;
}

