#define declare_ch_vector(void) 

struct ch_vector;
typedef struct ch_vector ch_vector_t;


struct ch_vector{
    ch_word size;  /*Return the max number number of elements in the vector list*/
    ch_word count;  /*Return the actual number of elements in the vector*/
    void (*resize)(ch_vector_t* this, ch_word new_size); /*Resize the vector*/
    ch_word (*eq)(ch_vector_t* this, ch_vector_t* that); /*Check for equality*/

    void* (*off)(ch_vector_t* this, ch_word idx); /*Return the element at a given offset, with bounds checking*/
    void* first; /*Pointer to the fist valid entry list. Not valid if first == end*/
    void* last; /*Pointer to the last valid element in the list. Not valid if last == end*/
    void* end; /*Pointer to the one element beyond the end of the valid elements in list. Do not dereference! */

    void* (*next)(ch_vector_t* this, void* ptr);  /*Step forwards by one entry*/ 
    void* (*prev)(ch_vector_t* this, void* ptr); /*Step backwards by one entry*/
    void* (*forward)(ch_vector_t* this, void* ptr, ch_word amount);  /*Step forwards by amount*/ 
    void* (*back)(ch_vector_t* this, void* ptr, ch_word amount);  /*Step backwards by amount*/ 

    void* (*find)(ch_vector_t* this, void* begin, void* end, void value); /*find the given value using the comparator function*/
    void (*sort)(ch_vector_t* this); /*sort into order given the comparator function*/
    void (*sort_reverse)(ch_vector_t* this); /*sort into reverse order given the comparator function*/

    void* (*push_front)(ch_vector_t* this, void value); /* Put an element at the front of the vector list values, [WARN: In general this is very expensive for an vector] */
    void (*pop_front)(ch_vector_t* this); /* Push an element off the front of the vector list values, [WARN: In general this is very expensive for an vector] */
    void* (*push_back)(ch_vector_t* this, void value); /* Put an element at the back of the vector values*/
    void (*pop_back)(ch_vector_t* this); /* Push an element at the back of the vector values*/

    void* (*insert_after)(ch_vector_t* this, void* ptr, void value); /* Insert an element after the element given by ptr*/
    void* (*insert_before)(ch_vector_t* this, void* ptr, void value); /* Insert an element before the element giver by ptr [WARN: In general this is very expensive for an vector] */
    void* (*remove)(ch_vector_t* this, void* ptr); /*Remove the given ptr [WARN: In general this is very expensive] */

    void (*delete)(ch_vector_t* this); /*Free the resources associated with this vector, assumes that individual items have been freed*/ 

    void* (*push_back_carray)(ch_vector_t* this, void* cvector, ch_word count); /*Push back count elements the C vector to the back vector-list*/

     /* Members prefixed with "_" are nominally "private" Don't touch my privates!*/
    ch_word (*_cmp)(void lhs, void rhs); /* Comparator function for find and sort operations */
    ch_array_t* _array_backing; /*Actual vector storage*/
    ch_word _array_backing_count; /*NUmber of elements currently in the vector*/

};


ch_vector_t* ch_vector_new(ch_word size, ch_word(*cmp)(void lhs, void rhs) )
