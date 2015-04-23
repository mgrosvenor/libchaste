#include "array.h"
#include "../../utils/util.h"
#include "../../types/types.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#define _LAST ( _array_forward_unsafe(this,this->_array_backing, this->_array_backing_size -1))


//These are for use when we are still initilising everything. Do not use generally
static void* _array_forward_unsafe(ch_array_t* this, void* ptr, ch_word amount)
{
    return (ch_byte*)ptr + (amount * this->_element_size);
}

static void* _array_next_unsafe(ch_array_t* this, void* ptr)
{
    return _array_forward_unsafe(this, ptr, 1);
}



void* array_forward(ch_array_t* this, void* ptr, ch_word amount)
{
    if((ch_byte*)ptr + amount * this->_element_size <= (ch_byte*)this->end){
        return (ch_byte*)ptr + (amount * this->_element_size);
    }

    printf("Array forward catch\n");
    return ptr;
}

void* array_back(ch_array_t* this, void* ptr, ch_word amount)
{
    if((ch_byte*)ptr - amount * this->_element_size >= (ch_byte*)this->first){
        return (ch_byte*)ptr - amount * this->_element_size;
    }

    printf("Array backward catch\n");
    return ptr;
}



void* array_next(ch_array_t* this, void* ptr)
{
    return array_forward(this, ptr, 1);
}

void* array_prev(ch_array_t* this, void* ptr)
{
    return array_back(this, ptr, 1);
}



void array_resize(ch_array_t* this, ch_word new_size)
{
    this->_array_backing = (ch_byte*)realloc(this->_array_backing, new_size * this->_element_size);
    if(!this->_array_backing){
        printf("Could not allocate memory for backing store\n");
        return;
    }

    this->_array_backing_size  = new_size;
    this->size                 = new_size;

    if(new_size == 0){
        this->first          = this->_array_backing;
        this->last           = this->_array_backing;
        this->end            = this->_array_backing;
    }
    else{
        this->first            = this->_array_backing;
        this->last             = _LAST;
        this->end              = _array_next_unsafe(this,this->last);
    }

}


//Check for equality between two array lists
ch_word array_eq(ch_array_t* this, ch_array_t* that)
{

    if(this->_array_backing_size != that->_array_backing_size){
        return 0; 
    }

    void* i = this->first;
    void* j = that->first;

    for(; i < this->end && j < that->end; i = array_next(this, i), j = array_next(that, j)){
        if( this->_cmp(i,j) ){
            return 0;
        }
    }

    return 1;
}


//Take an index an fix it so that negative indexs are legal
static inline ch_word range_fix(ch_array_t* this, ch_word idx)
{
    idx = idx < 0 ? idx + this->_array_backing_size : idx;

    if(idx >= 0 && idx < this->_array_backing_size){
        return idx;
    }

    printf("Index (%lli) is out of the valid range [%lli,%lli]\n", idx, -1 * this->_array_backing_size, this->_array_backing_size - 1 );
    return -1;

}


//Return the element at a given offset, with bounds checking
void* array_off(ch_array_t* this, ch_word idx)
{

    if(this->_array_backing_size < 1){
        printf("Index (%lli) is out of the valid range [%lli,%lli]\n", idx, -1 * this->_array_backing_size, this->_array_backing_size - 1 );
        return NULL;
    }

    idx = range_fix(this, idx);
    if(idx >= 0){
        return ((ch_byte*)this->_array_backing + this->_element_size * idx);
    }

    return NULL;

}



//find the given value using the comparator function
void* array_find(ch_array_t* this, void* begin, void* end, void* value)
{
    if(unlikely(begin < this->first || begin > this->end)){
        printf("Begin iterator supplied is out of range is not a member of this array\n");
        return NULL;
    }

    if(unlikely(end < this->first || end > this->end)){
        printf("End iterator supplied is out of range is not a member of this array\n");
        return NULL;
    }

    for(void* it = begin; it != end; it = array_next(this,it)){
        if(this->_cmp(it,value) == 0 ){
            return it;
        }
    }

    return NULL;
}


//Sort into order given the comparator function
void array_sort(ch_array_t* this)
{
    qsort(this->_array_backing, this->_array_backing_size, this->_element_size, this->_cmp);
}



void array_delete(ch_array_t* this)
{
    if(this->_array_backing){
        free(this->_array_backing);
    }

    free(this);
}


//Assign at most size elements from the C array
void* array_from_carray(ch_array_t* this, void* carray, ch_word count)
{
    count = MIN(count,this->_array_backing_size);
    memcpy(this->first, carray,  count * this->_element_size );

    return this->last;
}


ch_array_t* ch_array_new(ch_word element_count, ch_word element_size, cmp_void_f cmp)
{

    ch_array_t* result = (ch_array_t*)calloc(1,sizeof(ch_array_t));
    if(!result){
        printf("Could not allocate memory for new array structure. Giving up\n");
        return NULL;
    }

    const ch_word page_size             = sysconf(_SC_PAGE_SIZE);
    const ch_word alignment             = page_size; //Make everything page size aligned, which will also be "cache" aligned
    //const ch_word aligned_element_size  = ((element_size + alignment -1) / alignment ) * alignment;
    const ch_word aligned_array_bytes   = next_pow2( element_size * element_count);
    //const ch_word aligned_element_count = aligned_array_bytes / aligned_element_size;

    //printf("\nRequested %li elements of %li bytes each -- ", element_count, element_size);
    //printf("Allocating %li elements of %li bytes each, for a total of %li MBytes\n", element_count, element_size, aligned_array_bytes / 1024 / 1024 );

    if(element_count > 0){

        int error = posix_memalign(&result->_array_backing, alignment, aligned_array_bytes);
        if( error || !result->_array_backing){
            printf("Could not allocate memory for new array backing. Giving up\n");
            free(result);
            return NULL;
        }
        memset(result->_array_backing,0,aligned_array_bytes);
    }
    else{
        result->_array_backing = NULL;
    }

    //We have memory to play with, now do all the other assignments
    result->_array_backing_size     = element_count;
    result->_element_size           = element_size;
    result->_cmp                    = cmp;
    result->first                   = result->_array_backing;
    result->last                    = result->first ? _array_forward_unsafe(result,result->first, element_count -1) : NULL;
    result->end                     = result->first ? _array_next_unsafe(result,result->last): NULL;
    result->size                    = result->_array_backing_size;

    return result;
}
