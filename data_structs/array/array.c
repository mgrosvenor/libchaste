#include "array.h"
#include "../../utils/util.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define _LAST (_forward(this,this->_array_backing, this->_array_backing_size -1))

static void* _forward(ch_array_t* this, void* ptr, ch_word amount)
{
    if((ch_byte*)ptr + amount * this->_element_size <= (ch_byte*)this->end){
        return (ch_byte*)ptr + (amount * this->_element_size);
    }

    return ptr;
}

static void* _back(ch_array_t* this, void* ptr, ch_word amount)
{
    if((ch_byte*)ptr - amount * this->_element_size >= (ch_byte*)this->first){
        return (ch_byte*)ptr - amount * this->_element_size;
    }

    return ptr;
}



static void* _next(ch_array_t* this, void* ptr)
{
    return _forward(this, ptr, 1);
}

static void* _prev(ch_array_t* this, void* ptr)
{
    return _back(this, ptr, 1);
}



static void _resize(ch_array_t* this, ch_word new_size)
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
        this->end              = _next(this,this->last);
    }

}


/*Check for equality between two array lists*/
static ch_word _eq(ch_array_t* this, ch_array_t* that)
{

    if(this->_array_backing_size != that->_array_backing_size){
        return 0; 
    }

    void* i = this->first;
    void* j = that->first;

    for(; i < this->end && j < that->end; i = this->next(this, i), j = that->next(that, j)){
        if( this->_cmp(i,j) ){
            return 0;
        }
    }

    return 1;
}


/*Take an index an fix it so that negative indexs are legal */
static inline ch_word range_fix(ch_array_t* this, ch_word idx)
{
    idx = idx < 0 ? idx + this->_array_backing_size : idx;

    if(idx >= 0 && idx < this->_array_backing_size){
        return idx;
    }

    printf("Index (%li) is out of the valid range [%li,%li]\n", idx, -1 * this->_array_backing_size, this->_array_backing_size - 1 );
    return -1;

}


/*Return the element at a given offset, with bounds checking*/
static void* _off(ch_array_t* this, ch_word idx)
{

    if(this->_array_backing_size < 1){
        printf("Index (%li) is out of the valid range [%li,%li]\n", idx, -1 * this->_array_backing_size, this->_array_backing_size - 1 );
        return NULL;
    }

    idx = range_fix(this, idx);
    if(idx >= 0){
        return &this->_array_backing[idx];
    }

    return NULL;

}





/*find the given value using the comparator function*/
static void* _find(ch_array_t* this, void* begin, void* end, void* value)
{
    if(unlikely(begin < this->first || begin > this->end)){
        printf("Begin iterator supplied is out of range is not a member of this array\n");
        return NULL;
    }

    if(unlikely(end < this->first || end > this->end)){
        printf("End iterator supplied is out of range is not a member of this array\n");
        return NULL;
    }

    for(void* it = begin; it != end; it = _next(this,it)){
        if(this->_cmp(it,value) == 0 ){
            return it;
        }
    }

    return NULL;
}



/* Merge two sorted arrays into out. */
static inline void* _merge(ch_array_t* this, void* out, void* lhs_lo, void* lhs_hi, void* rhs_lo, void* rhs_hi, ch_word dir)
{

    void* lhs_ptr = lhs_lo;
    void* rhs_ptr = rhs_lo;

    while(1){

        while(lhs_ptr <= lhs_hi &&  dir * this->_cmp(lhs_ptr,rhs_ptr) <= 0){
            lhs_ptr = _next(this,lhs_ptr);
        }


        const ch_word lhs_cpy_bytes  = (ch_byte*)lhs_ptr - (ch_byte*)lhs_lo;
        memcpy(out,lhs_lo,lhs_cpy_bytes);
        lhs_lo = lhs_ptr;
        out = (ch_byte*)out + lhs_cpy_bytes;

        if(lhs_lo > lhs_hi){
            break;
        }

        while(rhs_ptr <= rhs_hi && dir * this->_cmp(rhs_ptr,lhs_ptr) <= 0){
            rhs_ptr = _next(this,rhs_ptr);
        }

        const ch_word rhs_cpy_bytes = (ch_byte*)rhs_ptr - (ch_byte*)rhs_lo;
        memcpy(out,rhs_lo,rhs_cpy_bytes);
        rhs_lo = rhs_ptr;
        out = (ch_byte*)out + rhs_cpy_bytes;

        if(rhs_lo > rhs_hi){
            break;
        }
    }



    if(lhs_lo <= lhs_hi){
        const ch_word lhs_cpy_bytes = (ch_byte*)lhs_hi - (ch_byte*)lhs_lo + 1;
        memcpy(out,lhs_lo,lhs_cpy_bytes);
        out = (ch_byte*)out + lhs_cpy_bytes;
    }

    if(rhs_lo <= rhs_hi){
        const ch_word rhs_cpy_bytes = (ch_byte*)rhs_hi - (ch_byte*)rhs_lo + 1;
        memcpy(out,rhs_lo,rhs_cpy_bytes);
        out = (ch_byte*)out + rhs_cpy_bytes;
    }

    return out;

}



/*sort into order given the comparitor function dir -1 = down, 1 = up*/
static void _sort_dir(ch_array_t* this, ch_word dir)
{
    if(this->_array_backing_size <= 1){
        return; /*Nothing to do here. */
    }

    /* Grab some temporary auxilary storage */
    void* aux1 = (void*)malloc(this->_array_backing_size * this->_element_size);
    if(!aux1){
        printf("Could not allocate memory for new array structure. Giving up\n");
        return;
    }
    void* dst = aux1;
    void* src = this->_array_backing;

    ch_word chunk_size = 1;
    while(chunk_size <= this->_array_backing_size){
        void* dst_ptr = dst;
        void* lhs_lo  = src;
        void* lhs_hi  = this->forward(this, lhs_lo, chunk_size - 1);
        void* rhs_lo  = this->next(this, lhs_hi);
        void* rhs_hi  = this->forward(this, rhs_lo,  chunk_size - 1);

        while(1){
            if(lhs_lo > _LAST){
                break;
            }

            if(rhs_lo <= _LAST && rhs_hi > _LAST){
                rhs_hi = _LAST;
            }

            if(lhs_lo <= _LAST && lhs_hi > _LAST){
                lhs_hi = _LAST;
            }


            /* There is no right hand side, so just copy the lhs bytes */
            if(rhs_lo > _LAST){
                const ch_word lhs_cpy_bytes = (ch_byte*)lhs_hi - (ch_byte*)lhs_lo + 1;
                memcpy(dst_ptr,lhs_lo, lhs_cpy_bytes );
                break;
            }

            dst_ptr = _merge(this,dst_ptr,lhs_lo, lhs_hi, rhs_lo, rhs_hi, dir);


            lhs_lo = this->next(this, rhs_hi);
            lhs_hi = this->forward(this, lhs_lo, chunk_size - 1);
            rhs_lo = this->next(this, lhs_hi);
            rhs_hi = this->forward(this, rhs_lo,  chunk_size - 1);
        }

        chunk_size *= 2;
        if(dst == aux1){
            src = aux1;
            dst = this->_array_backing;
        }
        else{
            src = this->_array_backing;
            dst = aux1;
        }
    }


    if(dst == this->_array_backing){
        memcpy(dst, src, this->_array_backing_size * this->_element_size);
    }

    free(aux1);

}


/*sort into reverse order given the comparitor function*/
static void _sort(ch_array_t* this)
{
    _sort_dir(this,1);
}


/*sort into reverse order given the comparitor function*/
static void _sort_reverse(ch_array_t* this)
{
    _sort_dir(this,-1);
}



static void _delete(ch_array_t* this)
{
    if(this->_array_backing){
        free(this->_array_backing);
    }

    free(this);
}


/*Assign at most size elements from the C array*/
static void* _from_carray(ch_array_t* this, void* carray, ch_word count)
{
    count = MIN(count,this->_array_backing_size);
    memcpy(this->first, carray,  count * this->_element_size );

    return this->last;
}


ch_array_t* ch_array_new_generic(ch_word size, ch_word element_size, ch_word(*cmp)(void* lhs, void* rhs))
{

    ch_array_t* result = (ch_array_t*)calloc(1,sizeof(ch_array_t));
    if(!result){
        printf("Could not allocate memory for new array structure. Giving up\n");
        return NULL;
    }



    if(size > 0){
        result->_array_backing       = calloc(size,element_size);
        if(!result->_array_backing){
            printf("Could not allocate memory for new array backing. Giving upn");
            free(result);
            return NULL;
        }
    }
    else{
        result->_array_backing = NULL;
    }

    /*We have memory to play with, now do all the other assignments*/
    result->_array_backing_size     = size;
    result->_element_size           = element_size;
    result->_cmp                    = cmp;
    result->first                   = result->_array_backing;
    result->last                    = result->first ? _forward(result,result->first, size -1) : NULL;
    result->end                     = result->first ? _next(result,result->last): NULL;
    result->size                    = result->_array_backing_size;
    result->resize                  = _resize;
    result->eq                      = _eq;
    result->off                     = _off;
    result->next                    = _next;
    result->prev                    = _prev;
    result->forward                 = _forward;
    result->back                    = _back;
    result->find                    = _find;
    result->sort                    = _sort;
    result->sort_reverse            = _sort_reverse;
    result->from_carray             = _from_carray;
    result->delete                  = _delete;

    return result;
}
