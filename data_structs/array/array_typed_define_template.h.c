#define define_array(i64)

static void _update_i64(ch_array_i64_t* this)
{
    this->size  = this->_array->size;
    this->first = this->_array->first;
    this->last  = this->_array->last;
    this->end   = this->_array->end;
}

static void _resize_i64(ch_array_i64_t* this, ch_word new_size)                 { array_resize(this->_array, new_size); _update_i64(this); }
static ch_word _eq_i64(ch_array_i64_t* this, ch_array_i64_t* that)              { ch_word result = array_eq(this->_array, that->_array); _update_i64(this); return result;}
static i64* _off_i64(ch_array_i64_t* this, ch_word idx)                         { i64* result = (i64*)array_off(this->_array, idx); _update_i64(this); return result; }
static i64* _forward_i64(ch_array_i64_t* this, i64* ptr, ch_word amount)        { i64* result = (i64*)array_forward(this->_array, (void*)ptr, amount); _update_i64(this); return result; }
static i64* _back_i64(ch_array_i64_t* this, i64* ptr, ch_word amount)           { i64* result =  (i64*)array_back(this->_array, (void*)ptr, amount); _update_i64(this); return result; }
static i64* _next_i64(ch_array_i64_t* this, i64* ptr)                           { i64* result = (i64*)_forward_i64(this, ptr, 1); _update_i64(this); return result; }
static i64* _prev_i64(ch_array_i64_t* this, i64* ptr)                           { i64* result = (i64*)_back_i64(this, ptr, 1); _update_i64(this); return result; }
static i64* _find_i64(ch_array_i64_t* this, i64* begin, i64* end, i64 value)    { i64* result = (i64*) array_find(this->_array, (void*)begin, (void*)end, &value); _update_i64(this); return result; }
static void _sort_i64(ch_array_i64_t* this)                                     { array_sort(this->_array); _update_i64(this); }
static i64* _from_carray_i64(ch_array_i64_t* this, i64* carray, ch_word count)  { i64* result =  array_from_carray(this->_array, (void*)carray, count); _update_i64(this); return result; }

static void _delete_i64(ch_array_i64_t* this)
{
    if(this->_array){
        array_delete(this->_array);
    }

    free(this);
}


ch_array_i64_t* ch_array_i64_new(ch_word size, ch_word (*cmp)(i64* lhs, i64* rhs) )
{

    ch_array_i64_t* result = (ch_array_i64_t*)calloc(1,sizeof(ch_array_i64_t));
    if(!result){
        printf("Could not allocate memory for new array structure. Giving upn");
        return ((void *)0);
    }

    result->_array = ch_array_new(size, sizeof(i64), (cmp_void_f)cmp );


    /*We have memory to play with, now do all the other assignments*/
    result->resize                  = _resize_i64;
    result->off                     = _off_i64;
    result->next                    = _next_i64;
    result->prev                    = _prev_i64;
    result->forward                 = _forward_i64;
    result->back                    = _back_i64;
    result->from_carray             = _from_carray_i64;
    result->delete                  = _delete_i64;

    /*Fail hard and early if the compare function is NULL*/
    if(cmp){
        result->eq                      = _eq_i64;
        result->find                    = _find_i64;
        result->sort                    = _sort_i64;
    }

    _update_i64(result);

    return result;
