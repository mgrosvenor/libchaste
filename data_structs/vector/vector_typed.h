#include "../../types/types.h"
#include "vector.h"

#define declare_ch_vector(void) 

typedef ch_word (*cmp_void_f)(void*, void*);

struct ch_vector_i64;
typedef struct ch_vector_i64 ch_vector_i64_t;


struct ch_vector_i64{
    ch_word size;  /*Return the max number number of elements in the vector list*/
    ch_word count;  /*Return the actual number of elements in the vector*/
    i64* first; /*Pointer to the fist valid entry list. Not valid if first == end*/
    i64* last; /*Pointer to the last valid element in the list. Not valid if last == end*/
    i64* end; /*Pointer to the one element beyond the end of the valid elements in list. Do not dereference! */


    void (*resize)(ch_vector_i64_t* this, ch_word new_size); /*Resize the vector*/
    i64* (*off)(ch_vector_i64_t* this, ch_word idx); /*Return the element at a given offset, with bounds checking*/

    i64* (*next)(ch_vector_i64_t* this, i64* ptr);  /*Step forwards by one entry*/
    i64* (*prev)(ch_vector_i64_t* this, i64* ptr); /*Step backwards by one entry*/
    i64* (*forward)(ch_vector_i64_t* this, i64* ptr, ch_word amount);  /*Step forwards by amount*/
    i64* (*back)(ch_vector_i64_t* this, i64* ptr, ch_word amount);  /*Step backwards by amount*/

    i64* (*push_front)(ch_vector_i64_t* this, i64 value); /* Put an element at the front of the vector list values, [WARN: In general this is very expensive for an vector] */
    void (*pop_front)(ch_vector_i64_t* this); /* Push an element off the front of the vector list values, [WARN: In general this is very expensive for an vector] */
    i64* (*push_back)(ch_vector_i64_t* this, i64 value); /* Put an element at the back of the vector values*/
    void (*pop_back)(ch_vector_i64_t* this); /* Push an element at the back of the vector values*/

    i64* (*insert_after)(ch_vector_i64_t* this, i64* ptr, i64 value); /* Insert an element after the element given by ptr*/
    i64* (*insert_before)(ch_vector_i64_t* this, i64* ptr, i64 value); /* Insert an element before the element giver by ptr [WARN: In general this is very expensive for an vector] */
    i64* (*remove)(ch_vector_i64_t* this, i64* ptr); /*Remove the given ptr [WARN: In general this is very expensive] */
    void (*delete)(ch_vector_i64_t* this); /*Free the resources associated with this vector, assumes that individual items have been freed*/

    i64* (*find)(ch_vector_i64_t* this, i64* begin, i64* end, i64 value); /*find the given value using the comparator function*/
    void (*sort)(ch_vector_i64_t* this); /*sort into order given the comparator function*/
    ch_word (*eq)(ch_vector_i64_t* this, ch_vector_i64_t* that); /*Check for equality*/

    i64* (*push_back_carray)(ch_vector_i64_t* this, i64* carray, ch_word count); /*Push back count elements the C vector to the back vector-list*/

     /* Members prefixed with "_" are nominally "private" Don't touch my privates!*/
    ch_vector_t* _vector; /*Actual vector storage*/

};


ch_vector_i64_t* ch_vector_i64_new(ch_word size, ch_word(*cmp)(i64* lhs, i64* rhs) );
