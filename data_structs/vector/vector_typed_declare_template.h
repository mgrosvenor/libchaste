#ifndef VECTOR_TYPED_TEMPLATE_DECLARE_H_
#define VECTOR_TYPED_TEMPLATE_DECLARE_H_


#include "../../types/types.h"
#include "vector.h"

typedef ch_word (*cmp_void_f)(void*, void*);

#define declare_ch_vector(NAME,TYPE) \
\
struct ch_vector_##NAME;\
typedef struct ch_vector_##NAME ch_vector_##NAME##_t;\
\
\
struct ch_vector_##NAME{\
    ch_word size;  /*Return the max number number of elements in the vector list*/\
    ch_word count;  /*Return the actual number of elements in the vector*/\
    TYPE* first; /*Pointer to the fist valid entry list. Not valid if first == end*/\
    TYPE* last; /*Pointer to the last valid element in the list. Not valid if last == end*/\
    TYPE* end; /*Pointer to the one element beyond the end of the valid elements in list. Do not dereference! */\
\
\
    void (*resize)(ch_vector_##NAME##_t* this, ch_word new_size); /*Resize the vector*/\
    TYPE* (*off)(ch_vector_##NAME##_t* this, ch_word idx); /*Return the element at a given offset, with bounds checking*/\
\
    TYPE* (*next)(ch_vector_##NAME##_t* this, TYPE* ptr);  /*Step forwards by one entry*/\
    TYPE* (*prev)(ch_vector_##NAME##_t* this, TYPE* ptr); /*Step backwards by one entry*/\
    TYPE* (*forward)(ch_vector_##NAME##_t* this, TYPE* ptr, ch_word amount);  /*Step forwards by amount*/\
    TYPE* (*back)(ch_vector_##NAME##_t* this, TYPE* ptr, ch_word amount);  /*Step backwards by amount*/\
\
    TYPE* (*push_front)(ch_vector_##NAME##_t* this, TYPE value); /* Put an element at the front of the vector list values, [WARN: In general this is very expensive for an vector] */\
    void (*pop_front)(ch_vector_##NAME##_t* this); /* Push an element off the front of the vector list values, [WARN: In general this is very expensive for an vector] */\
    TYPE* (*push_back)(ch_vector_##NAME##_t* this, TYPE value); /* Put an element at the back of the vector values*/\
    void (*pop_back)(ch_vector_##NAME##_t* this); /* Push an element at the back of the vector values*/\
\
    TYPE* (*insert_after)(ch_vector_##NAME##_t* this, TYPE* ptr, TYPE value); /* Insert an element after the element given by ptr*/\
    TYPE* (*insert_before)(ch_vector_##NAME##_t* this, TYPE* ptr, TYPE value); /* Insert an element before the element giver by ptr [WARN: In general this is very expensive for an vector] */\
    TYPE* (*remove)(ch_vector_##NAME##_t* this, TYPE* ptr); /*Remove the given ptr [WARN: In general this is very expensive] */\
    void (*delete)(ch_vector_##NAME##_t* this); /*Free the resources associated with this vector, assumes that individual items have been freed*/\
\
    TYPE* (*find)(ch_vector_##NAME##_t* this, TYPE* begin, TYPE* end, TYPE value); /*find the given value using the comparator function*/\
    void (*sort)(ch_vector_##NAME##_t* this); /*sort into order given the comparator function*/\
    ch_word (*eq)(ch_vector_##NAME##_t* this, ch_vector_##NAME##_t* that); /*Check for equality*/\
\
    TYPE* (*push_back_carray)(ch_vector_##NAME##_t* this, TYPE* carray, ch_word count); /*Push back count elements the C vector to the back vector-list*/\
\
     /* Members prefixed with "_" are nominally "private" Don't touch my privates!*/\
    ch_vector_t* _vector; /*Actual vector storage*/\
\
};\
\
\
ch_vector_##NAME##_t* ch_vector_##NAME##_new(ch_word size, ch_word(*cmp)(TYPE* lhs, TYPE* rhs) );

#define declare_ch_vector_cmp(NAME, TYPE) ch_word ch_vector_cmp_##NAME(TYPE* lhs, TYPE* rhs);

#define CH_VECTOR(NAME)  ch_vector_##NAME##_t
#define CH_VECTOR_NEW(NAME, size, cmp) ch_vector_##NAME##_new(size, cmp )
#define CH_VECTOR_CMP(NAME) ch_vector_cmp_##NAME

#endif //VECTOR_TYPED_TEMPLATE_DECLARE_H_

