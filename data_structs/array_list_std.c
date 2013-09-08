/*
 * array_std_int.c
 *
 *  Created on: 6 Sep 2013
 *      Author: mpg39
 */


#include "array_list_define_tempate.h"

define_ch_array(test_t)
define_ch_array(int)
define_ch_array(short)
define_ch_array(long)
define_ch_array(char)
define_ch_array(float)
//define_ch_array(TYPE)

static void _resize##TYPE(ch_array##TYPE##t* this, ch_word new_size)
{
    this->_array_backing = (TYPE*)realloc(this->_array_backing, new_size * sizeof(TYPE));
    if(!this->_array_backing){
        printf("Could not allocate memory for backing storen");
        return;
    }

    if(new_size < this->_array_backing_size){
        this->end = this->_array_backing + new_size;
        this->last = this->end - 1;
        this->_array_backing_count = new_size;
        this->count = this->_array_backing_count;
    }

    this->_array_backing_size = new_size;
    this->size = new_size; /* Update the public size*/

}

/*Take an index an fix it so that negative indexs are legal */
static inline ch_word range_fix##TYPE(ch_array##TYPE##t* this, ch_word idx)
{
    idx = idx < 0 ? idx + this->_array_backing_size : idx;

    if(idx >= 0 && idx < this->_array_backing_count && idx < this->_array_backing_size){
        return idx;
    }

    printf("Index (%li) is out of the valid range [%li,%li]", idx, -1 * this->_array_backing_size, this->_array_backing_size - 1 );
    return -1;

}


/*Return the element at a given offset, with bounds checking*/
static TYPE* _off##TYPE(ch_array##TYPE##t* this, ch_word idx)
{

    idx = range_fix##TYPE(this, idx);
    if(idx >= 0){
        return &this->_array_backing[idx];
    }

    return NULL;

}


static TYPE* _forward##TYPE(ch_array##TYPE##t* this, TYPE* ptr, ch_word amount)
{
    if(ptr + amount <= this->end){
        return ptr += amount;
    }

    return ptr;
}

static TYPE* _back##TYPE(ch_array##TYPE##t* this, TYPE* ptr, ch_word amount)
{
    if(ptr - amount >= this->first){
        return ptr -= amount;
    }

    return ptr;
}



static TYPE* _next##TYPE(ch_array##TYPE##t* this, TYPE* ptr)
{
    return _forward##TYPE(this, ptr, 1);
}

static TYPE* _prev##TYPE(ch_array##TYPE##t* this, TYPE* ptr)
{
    return _back##TYPE(this, ptr, 1);
}



/*find the given value using the comparitor function*/
static TYPE* _find##TYPE(ch_array##TYPE##t* this, TYPE* begin, TYPE* end, TYPE value)
{
    for(TYPE* it = begin; it != end; it++){
        if(this->_cmp(*it,value) == 0 ){
            return it;
        }
    }

    return NULL;
}


//static void dump_array(ch_array##TYPE##t* al)
//{
//    printf("There are %li items in the list\n", al->count);
//    for(TYPE* i = al->first; i < al->end; i = al->next(al, i)){
//        printf("%lf,", *i);
//    }
//    printf("\n");
//}



/* Merge two sorted arrays into out. */
static inline TYPE* _merge##TYPE(TYPE* out, TYPE* lhs_lo, TYPE* lhs_hi, TYPE* rhs_lo, TYPE* rhs_hi, ch_word (*cmp)(TYPE lhs, TYPE rhs), ch_word dir)
{

    TYPE* lhs_ptr = lhs_lo;
    TYPE* rhs_ptr = rhs_lo;

    while(1){

        while(lhs_ptr <= lhs_hi &&  dir * cmp(*lhs_ptr,*rhs_ptr) <= 0){
            lhs_ptr++;
        }

        const ch_word lhs_cpy_count = lhs_ptr - lhs_lo;
        const ch_word lhs_cpy_bytes  = lhs_cpy_count * sizeof(TYPE);
        memcpy(out,lhs_lo,lhs_cpy_bytes);
        lhs_lo = lhs_ptr;
        out += lhs_cpy_count;

        if(lhs_lo > lhs_hi){
            break;
        }

        while(rhs_ptr <= rhs_hi && dir * cmp(*rhs_ptr,*lhs_ptr) <= 0){
            rhs_ptr++;
        }

        const ch_word rhs_cpy_count = rhs_ptr - rhs_lo;
        const ch_word rhs_cpy_bytes  = rhs_cpy_count * sizeof(TYPE);
        memcpy(out,rhs_lo,rhs_cpy_bytes);
        rhs_lo = rhs_ptr;
        out += rhs_cpy_count;

        if(rhs_lo > rhs_hi){
            break;
        }
    }



    if(lhs_lo <= lhs_hi){
        const ch_word lhs_cpy_count = lhs_hi - lhs_lo + 1;
        const ch_word lhs_cpy_bytes  = lhs_cpy_count * sizeof(TYPE);
        memcpy(out,lhs_lo,lhs_cpy_bytes);
        out += lhs_cpy_count;
    }

    if(rhs_lo <= rhs_hi){
        const ch_word rhs_cpy_count = rhs_hi - rhs_lo + 1;
        const ch_word rhs_cpy_bytes  = rhs_cpy_count * sizeof(TYPE);
        memcpy(out,rhs_lo,rhs_cpy_bytes);
        out += rhs_cpy_count;
    }

    return out;

}

#define _LAST(a) (a + this->_array_backing_count -1)


/*sort into order given the comparitor function dir -1 = down, 1 = up*/
static void _sort_dir##TYPE(ch_array##TYPE##t* this, ch_word dir)
{
    if(this->_array_backing_count <= 1){
        return; /*Nothing to do here. */
    }

    /* Grab some temporary auxilary storage */
    TYPE* aux1 = (TYPE*)malloc(this->_array_backing_count * sizeof(TYPE));
    if(!aux1){
        printf("Could not allocate memory for new array structure. Giving up\n");
        return;
    }
    TYPE* dst = aux1;
    TYPE* src = this->_array_backing;

    ch_word chunk_size = 1;
    while(chunk_size <= this->_array_backing_count){
        TYPE* dst_ptr = dst;
        TYPE* lhs_lo  = src;
        TYPE* lhs_hi  = lhs_lo + chunk_size - 1;
        TYPE* rhs_lo  = lhs_hi + 1;
        TYPE* rhs_hi  = rhs_lo + chunk_size - 1;

        while(1){
            if(lhs_lo > _LAST(src)){
                break;
            }

            if(rhs_lo <= _LAST(src) && rhs_hi > _LAST(src)){
                rhs_hi = _LAST(src);
            }

            if(lhs_lo <= _LAST(src) && lhs_hi > _LAST(src)){
                lhs_hi = _LAST(src);
            }


            /* There is no right hand side, so just copy the lhs bytes */
            if(rhs_lo > _LAST(src)){
                const ch_word lhs_cpy_count = lhs_hi - lhs_lo + 1;
                const ch_word lhs_cpy_bytes  = lhs_cpy_count * sizeof(TYPE);
                memcpy(dst_ptr,lhs_lo, lhs_cpy_bytes );
                break;
            }

            dst_ptr = _merge##TYPE(dst_ptr,lhs_lo, lhs_hi, rhs_lo, rhs_hi, this->_cmp, dir);

            lhs_lo = rhs_hi + 1;
            lhs_hi = lhs_lo + chunk_size -1;
            rhs_lo = lhs_hi + 1;
            rhs_hi = rhs_lo + chunk_size - 1;
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
        memcpy(dst, src, this->_array_backing_count * sizeof(TYPE));
    }

    free(aux1);

}


/*sort into reverse order given the comparitor function*/
static void _sort##TYPE(ch_array##TYPE##t* this)
{
    _sort_dir##TYPE(this,1);
}


/*sort into reverse order given the comparitor function*/
static void _sort_reverse##TYPE(ch_array##TYPE##t* this)
{
    _sort_dir##TYPE(this,-1);
}


/* Insert an element befer the element giver by ptr [WARN: In general this is very expensive for an array] */
static TYPE* _insert_before##TYPE(ch_array##TYPE##t* this, TYPE* ptr, TYPE value)
{

    if(unlikely(ptr < this->_array_backing)){
        printf("ptr supplied is out of range. Too small.n");
        return NULL;
    }

    /* NB: It's ok to insert before count + 1. This essentially inserts at count which is the last item. */
    if(unlikely(ptr > this->_array_backing + this->_array_backing_count + 1)){
        printf("ptr supplied is out of range. Too big.n");
        return NULL;
    }

    if(unlikely(this->_array_backing_count >= this->_array_backing_size)){
        printf("Array list is full!n");
        return NULL;

    }

    if(likely(this->_array_backing_count  && ptr <= this->_array_backing + this->_array_backing_count )){
        memmove(ptr + 1,ptr , (this->_array_backing + this->_array_backing_count - ptr) * sizeof(TYPE));
    }

    *ptr = value;

    if(unlikely(this->_array_backing_count == 0)){
        this->last = this->_array_backing;
        this->first = this->_array_backing;
    }
    else{
        this->last++;
    }

    this->_array_backing_count++;
    this->count++;
    this->end++;

    return ptr;
}

/* Insert an element after the element given by ptr* [WARN: In general this is very expensive for an array] */
static TYPE* _insert_after##TYPE(ch_array##TYPE##t* this, TYPE* ptr, TYPE value)
{
    /*Inserting after is the equivalent to inserting before, the value after the current */
    return _insert_before##TYPE(this,ptr++,value);
}


/* Put an element at the front of the array values, [WARN: In general this is very expensive for an array] */
static TYPE* _push_front##TYPE(ch_array##TYPE##t* this, TYPE value)
{
    /* Pushing onto the front is equivalent to inserting at the head */
    return _insert_before##TYPE(this, this->_array_backing, value);
}

/* Put an element at the back of the arary values*/
static TYPE* _push_back##TYPE(ch_array##TYPE##t* this, TYPE value)
{
    /* Pushing onto the end is equivalent to inserting at the tail */
    _insert_before##TYPE(this, this->end, value);
    return NULL;
}


/*Remove the given ptr [WARN: In general this is very expensive], return an to the next item in the list */
static TYPE* _remove##TYPE(ch_array##TYPE##t* this, TYPE* ptr)
{

    if(unlikely(this->first == this->end)){
        printf("Array list is emptyn");
        return NULL;
    }

    if(unlikely(ptr < this->_array_backing)){
        printf("ptr supplied is out of range. Too small.n");
        return NULL;
    }

    if(unlikely(ptr > this->_array_backing + this->_array_backing_count)){
        printf("ptr supplied is out of range. Too big.n");
        return NULL;
    }

    /*Slow path*/
    if(unlikely(ptr != this->last)){
/*
        printf("ptr:%p[%li], first:%p[%li], last:%p[%li], end:%p:[%li]",
                (void*)ptr, ptr - this->first,
                (void*)this->first, this->last - this->first,
                (void*)this->last, this->last - this->first,
                (void*)this->end, this->end - this->first );
*/
        memmove(ptr, ptr + 1, (this->last) - (ptr + 1) );
    }

    this->_array_backing_count--;
    this->count--;


    if(unlikely(this->_array_backing_count == 0)){
        this->last  = this->_array_backing;
        this->first = this->_array_backing;
        this->end   = this->_array_backing;
    }
    else{
        this->last--;
        this->end--;
    }

    return ptr;
}


static void _delete##TYPE(ch_array##TYPE##t* this)
{
    if(this->_array_backing){
        free(this->_array_backing);
    }

    free(this);
}

ch_array##TYPE##t* ch_array##TYPE##new(ch_word size, ch_word (*cmp)(TYPE lhs, TYPE rhs) )
{

    ch_array##TYPE##t* result = (ch_array##TYPE##t*)calloc(1,sizeof(ch_array##TYPE##t));
    if(!result){
        printf("Could not allocate memory for new array structure. Giving upn");
        return NULL;
    }

    result->_array_backing       = calloc(size,sizeof(TYPE));
    if(!result->_array_backing){
        printf("Could not allocate memory for new array backing. Giving upn");
        free(result);
        return NULL;
    }

    /*We have memory to play with, now do all the other assignments*/
    result->_array_backing_size     = size;
    result->_array_backing_count    = 0;
    result->_cmp                    = cmp;
    result->first                   = result->_array_backing;
    result->last                    = result->_array_backing;
    result->end                     = result->_array_backing;
    result->size                    = result->_array_backing_size;
    result->count                   = result->_array_backing_count;
    result->resize                  = _resize##TYPE;
    result->off                     = _off##TYPE;
    result->next                    = _next##TYPE;
    result->prev                    = _prev##TYPE;
    result->forward                 = _forward##TYPE;
    result->back                    = _back##TYPE;
    result->find                    = _find##TYPE;
    result->sort                    = _sort##TYPE;
    result->sort_reverse            = _sort_reverse##TYPE;
    result->push_front              = _push_front##TYPE;
    result->push_back               = _push_back##TYPE;
    result->insert_after            = _insert_after##TYPE;
    result->insert_before           = _insert_before##TYPE;
    result->remove                  = _remove##TYPE;
    result->delete                  = _delete##TYPE;

    return result;
}
