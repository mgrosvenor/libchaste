#define define_ch_vector(void*) 

static void _resize_(ch_vector__t* this, ch_word new_size)
{
    this->_array_backing->resize(this->_array_backing,new_size);

    this->_array_backing_count = MIN(new_size, this->_array_backing_count);
    this->count                = this->_array_backing_count;
    this->size                 = new_size;

    if(new_size == 0){
        this->first          = NULL;
        this->last           = NULL;
        this->end            = NULL;
    }
    else{
        this->first            = this->_array_backing->first;
        this->end              = this->first + this->_array_backing_count;
        this->last             = this->end - 1;
    }

}


/*Check for equality between two vector lists*/
static ch_word _eq_(ch_vector__t* this, ch_vector__t* that)
{

    if(this->_array_backing_count != that->_array_backing_count){
        return 0;
    }

    void** i = this->first;
    void** j = that->first;
    for(; i < this->end && j < that->end; i = this->next(this, i), j = that->next(that, j)){
        if( this->_cmp(*i,*j) ){
            return 0;
        }
    }

    return 1;
}



/*Return the element at a given offset, with bounds checking*/
static void** _off_(ch_vector__t* this, ch_word idx)
{

    if(idx >= this->_array_backing_count){
        printf("Index (%li) is out of the valid range [%li,%li]", idx, -1 * this->_array_backing_count, this->_array_backing_count - 1 );
        return NULL;
    }

    return this->_array_backing->off(this->_array_backing,idx);

}


static void** _forward_(ch_vector__t* this, void** ptr, ch_word amount)
{
    if(ptr + amount <= this->end){
        return ptr += amount;
    }

    return ptr;
}

static void** _back_(ch_vector__t* this, void** ptr, ch_word amount)
{
    if(ptr - amount >= this->first){
        return ptr -= amount;
    }

    return ptr;
}



static void** _next_(ch_vector__t* this, void** ptr)
{
    return _forward_(this, ptr, 1);
}

static void** _prev_(ch_vector__t* this, void** ptr)
{
    return _back_(this, ptr, 1);
}



/*find the given value using the comparitor function*/
static void** _find_(ch_vector__t* this, void** begin, void** end, void* value)
{
    return this->_array_backing->find(this->_array_backing, begin, end, value);
}



/* Merge two sorted vectors into out. */
static inline void** _merge_(void** out, void** lhs_lo, void** lhs_hi, void** rhs_lo, void** rhs_hi, ch_word (*cmp)(void* lhs, void* rhs), ch_word dir)
{

    void** lhs_ptr = lhs_lo;
    void** rhs_ptr = rhs_lo;

    while(1){

        while(lhs_ptr <= lhs_hi &&  dir * cmp(*lhs_ptr,*rhs_ptr) <= 0){
            lhs_ptr++;
        }

        const ch_word lhs_cpy_count = lhs_ptr - lhs_lo;
        const ch_word lhs_cpy_bytes  = lhs_cpy_count * sizeof(void*);
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
        const ch_word rhs_cpy_bytes  = rhs_cpy_count * sizeof(void*);
        memcpy(out,rhs_lo,rhs_cpy_bytes);
        rhs_lo = rhs_ptr;
        out += rhs_cpy_count;

        if(rhs_lo > rhs_hi){
            break;
        }
    }



    if(lhs_lo <= lhs_hi){
        const ch_word lhs_cpy_count = lhs_hi - lhs_lo + 1;
        const ch_word lhs_cpy_bytes  = lhs_cpy_count * sizeof(void*);
        memcpy(out,lhs_lo,lhs_cpy_bytes);
        out += lhs_cpy_count;
    }

    if(rhs_lo <= rhs_hi){
        const ch_word rhs_cpy_count = rhs_hi - rhs_lo + 1;
        const ch_word rhs_cpy_bytes  = rhs_cpy_count * sizeof(void*);
        memcpy(out,rhs_lo,rhs_cpy_bytes);
        out += rhs_cpy_count;
    }

    return out;

}



/*sort into order given the comparitor function dir -1 = down, 1 = up*/
static void _sort_dir_(ch_vector__t* this, ch_word dir)
{
    if(this->_array_backing_count <= 1){
        return; /*Nothing to do here. */
    }

    /* Grab some temporary auxilary storage */
    void** aux1 = (void**)malloc(this->_array_backing_count * sizeof(void*));
    if(!aux1){
        printf("Could not allocate memory for new vector structure. Giving up\n");
        return;
    }
    void** dst = aux1;
    void** src = this->_array_backing->first;

    ch_word chunk_size = 1;
    while(chunk_size <= this->_array_backing_count){
        void** dst_ptr = dst;
        void** lhs_lo  = src;
        void** lhs_hi  = lhs_lo + chunk_size - 1;
        void** rhs_lo  = lhs_hi + 1;
        void** rhs_hi  = rhs_lo + chunk_size - 1;

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
                const ch_word lhs_cpy_bytes  = lhs_cpy_count * sizeof(void*);
                memcpy(dst_ptr,lhs_lo, lhs_cpy_bytes );
                break;
            }

            dst_ptr = _merge_(dst_ptr,lhs_lo, lhs_hi, rhs_lo, rhs_hi, this->_cmp, dir);

            lhs_lo = rhs_hi + 1;
            lhs_hi = lhs_lo + chunk_size -1;
            rhs_lo = lhs_hi + 1;
            rhs_hi = rhs_lo + chunk_size - 1;
        }

        chunk_size *= 2;
        if(dst == aux1){
            src = aux1;
            dst = this->_array_backing->first;
        }
        else{
            src = this->_array_backing->first;
            dst = aux1;
        }
    }


    if(dst == this->_array_backing->first){
        memcpy(dst, src, this->_array_backing_count * sizeof(void*));
    }

    free(aux1);

}


/*sort into reverse order given the comparitor function*/
static void _sort_(ch_vector__t* this)
{
    _sort_dir_(this,1);
}


/*sort into reverse order given the comparitor function*/
static void _sort_reverse_(ch_vector__t* this)
{
    _sort_dir_(this,-1);
}


/* Insert an element before the element giver by ptr [WARN: In general this is very expensive for an vector] */
static void** _insert_before_(ch_vector__t* this, void** ptr, void* value)
{

    /*If the backing memory is full, grow the vector*/
    if(unlikely(this->_array_backing_count == this->_array_backing->size)){
        const ch_word ptr_idx = ptr ? ptr - this->_array_backing->first: 0;
        const ch_word new_size = this->_array_backing->size ? this->_array_backing->size * 2 : 1;
        _resize_(this,new_size);
        ptr = this->_array_backing->first + ptr_idx;
    }

    if(unlikely(ptr < this->first)){
        printf("ptr supplied is out of range. Too small.\n");
        return NULL;
    }

    /* NB: It's ok to insert *before* last + 1. This essentially inserts at last which is the last item. */
    if(unlikely(ptr > this->last + 1)){
        printf("ptr supplied is out of range. Too big.\n");
        return NULL;
    }

    /* Optimise for the push_back case*/
    if(unlikely(this->_array_backing_count  && ptr <= this->_array_backing->first + this->_array_backing_count )){
        memmove(ptr + 1,ptr , (this->_array_backing->first + this->_array_backing_count - ptr) * sizeof(void*));
    }

    *ptr = value;

    if(unlikely(this->_array_backing_count == 0)){
        this->first = this->_array_backing->first;
        this->last  = this->first;
        this->end   = this->last + 1;
    }
    else{
        this->last++;
        this->end++;
    }

    this->_array_backing_count++;
    this->count++;

    return ptr;
}

/* Insert an element after the element given by ptr* [WARN: In general this is very expensive for an vector] */
static void** _insert_after_(ch_vector__t* this, void** ptr, void* value)
{
    /*Inserting after is the equivalent to inserting before, the value after the current */
    ptr = this->_array_backing->size == 0 ? NULL : ptr + 1;
    return _insert_before_(this,ptr,value);
}



/* Put an element at the front of the vector values, [WARN: In general this is very expensive for an vector] */
static void** _push_front_(ch_vector__t* this, void* value)
{
    /* Pushing onto the front is equivalent to inserting at the head */
    void** ptr = this->_array_backing->size == 0 ? NULL : this->first;
    return _insert_before_(this, ptr, value);
}

/* Put an element at the back of the arary values*/
static void** _push_back_(ch_vector__t* this, void* value)
{
    /* Pushing onto the end is equivalent to inserting at the tail */
    void** ptr = this->_array_backing->size == 0 ? NULL : this->end;
    return _insert_before_(this, ptr, value);
}


/*Remove the given ptr [WARN: In general this is very expensive], return an to the next item in the list */
static void** _remove_(ch_vector__t* this, void** ptr)
{

    if(unlikely(this->first == this->end)){
        printf("Array list is emptyn");
        return NULL;
    }

    if(unlikely(ptr < this->_array_backing->first)){
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
        memmove(ptr, ptr + 1, (this->last) - (ptr) );
    }

    this->_array_backing_count--;
    this->count--;


    if(unlikely(this->_array_backing_count == 0)){
        this->first = this->_array_backing->first;
        this->last = this->first;
        this->end  = this->last;
        ptr = NULL;
    }
    else{
        this->last--;
        this->end--;
    }

    return ptr;
}

/* Put an element at the front of the vector values, [WARN: In general this is very expensive for an vector] */
void _pop_front_(ch_vector__t* this)
{
    _remove_(this,this->first);
}

/* Put an element at the front of the vector values, [WARN: In general this is very expensive for an vector] */
void _pop_back_(ch_vector__t* this)
{
    _remove_(this,this->last);
}

static void _delete_(ch_vector__t* this)
{
    if(this->_array_backing){
        this->_array_backing->delete(this->_array_backing);
    }

    free(this);
}


/*Assign at most size elements from the C vector*/
static void** _push_back_carray_(ch_vector__t* this, void** cvector, ch_word count)
{
    if(this->_array_backing->size - this->_array_backing_count < count){
        const ch_word new_size = this->_array_backing->size ? this->_array_backing->size * 2 : next_pow2(count);
        _resize_(this,new_size);
    }

    memcpy(this->end, cvector, count * sizeof(void*) );
    this->_array_backing_count += count;
    this->count = this->_array_backing_count;
    this->last += count;
    this->end = this->last + 1;

    return this->last;
}


ch_vector__t* ch_vector__new(ch_word size, ch_word (*cmp)(void* lhs, void* rhs) )
{

    ch_vector__t* result = (ch_vector__t*)calloc(1,sizeof(ch_vector__t));
    if(!result){
        printf("Could not allocate memory for new vector structure. Giving up\n");
        return NULL;
    }

    result->_array_backing       = ch_array__new(size, cmp);

    /*We have memory to play with, now do all the other assignments*/
    result->_array_backing_count    = 0;
    result->_cmp                    = cmp;
    result->first                   = result->_array_backing->first;
    result->last                    = result->first;
    result->end                     = result->first;
    result->size                    = result->_array_backing->size;
    result->count                   = result->_array_backing_count;
    result->resize                  = _resize_;
    result->eq                      = _eq_;
    result->off                     = _off_;
    result->next                    = _next_;
    result->prev                    = _prev_;
    result->forward                 = _forward_;
    result->back                    = _back_;
    result->find                    = _find_;
    result->sort                    = _sort_;
    result->sort_reverse            = _sort_reverse_;
    result->push_front              = _push_front_;
    result->pop_front               = _pop_front_;
    result->push_back               = _push_back_;
    result->pop_front               = _pop_front_;
    result->insert_after            = _insert_after_;
    result->insert_before           = _insert_before_;
    result->remove                  = _remove_;
    result->push_back_carray        = _push_back_carray_;
    result->delete                  = _delete_;

    return result;
