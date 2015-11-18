/*
 * 
 *  Created:    Jul 30, 2015
 *  File name:  test_cbuff.c
 *  Description:
 *  <INSERT DESCRIPTION HERE> 
 */

#include "../data_structs/circular_buffer/circular_buffer.h"
#include "../utils/util.h"

#include <stdio.h>

//Check that basic construction and destruction works
static ch_word test1()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);

    cbuff_delete(cb1);
    return result;
}


//Add a single value to the structure
static ch_word test2()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    i64 val = 52;
    i64* val_p = cbuff_push_back(cb1, &val);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == val);

    cbuff_delete(cb1);
    return result;
}

//Test the peeking works as expected
static ch_word test3()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    i64 val = 52;
    i64* val_p = cbuff_push_back(cb1, &val);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == val);

    val_p = cbuff_peek_front(cb1);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == val);

    cbuff_delete(cb1);
    return result;
}

//Test the popping works as expected. Make sure that push pop push works
static ch_word test4()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    i64 val = 52;
    i64* val_p = cbuff_push_back(cb1, &val);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == val);

    cbuff_pop_front(cb1);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 1);
    CH_ASSERT(*val_p == val);

    i64* val_p2 = cbuff_push_back(cb1, &val);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 2);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 1);
    CH_ASSERT(*val_p == val);
    CH_ASSERT(*val_p2 == val);

    cbuff_delete(cb1);
    return result;
}


//Test pushing a small array
static ch_word test5()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    ch_word len = 2;
    i64 vals[2] = {52,53};
    i64* val_p = cbuff_push_back_carray(cb1, vals,&len);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 2);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 2);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == vals[0]);
    CH_ASSERT(len == 2);

    cbuff_delete(cb1);
    return result;
}

//Test pushing a large array
static ch_word test6()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(4,sizeof(i64));

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 4);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    ch_word len = 6;
    i64 vals[6] = {52,53,54,55,56,57};
    i64* val_p = cbuff_push_back_carray(cb1, vals,&len);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 4);
    CH_ASSERT(cb1->size == 4);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == vals[0]);
    CH_ASSERT(len == 4);

    cbuff_delete(cb1);
    return result;
}

//Test pushing a large array and then popping everything
static ch_word test7()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(4,sizeof(i64));

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 4);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    ch_word len = 6;
    i64 vals[6] = {52,53,54,55,56,57};
    i64* val_p = cbuff_push_back_carray(cb1, vals,&len);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 4);
    CH_ASSERT(cb1->size == 4);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == vals[0]);
    CH_ASSERT(len == 4);

    for(int i = 0; i < len; i++){
        cbuff_pop_front(cb1);
        CH_ASSERT(cb1->_array != NULL);
        CH_ASSERT(cb1->count == len - 1 - i);
        CH_ASSERT(cb1->size == 4 );
        CH_ASSERT(cb1->in_use == 0);
        CH_ASSERT(cb1->_add_next_index == 0);
        CH_ASSERT(cb1->_use_next_index == 0);
        const ch_word release_idx = (i + 1) % cb1->size;
        CH_ASSERT(cb1->_release_index == release_idx);
    }

    cbuff_delete(cb1);
    return result;
}

//Test pushing a small array and then using 1 thing
static ch_word test8()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);

    ch_word len = 2;
    i64 vals[6] = {52,53,54,55,56,57};
    i64* val_p = cbuff_push_back_carray(cb1, vals, &len);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 2);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 2);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == vals[0]);
    CH_ASSERT(len == 2);

    i64* used = cbuff_use_front(cb1);

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 2);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 1);
    CH_ASSERT(cb1->_add_next_index == 2);
    CH_ASSERT(cb1->_use_next_index == 1);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*used == vals[0]);

    cbuff_delete(cb1);
    return result;

}


//Test pushing a small array and then using all things
static ch_word test9()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));

    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);

    ch_word len = 2;
    i64 vals[6] = {52,53,54,55,56,57};
    i64* val_p = cbuff_push_back_carray(cb1, vals, &len);

    CH_ASSERT(cb1->count == 2);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 2);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == vals[0]);
    CH_ASSERT(len == 2);


    int i = 0;
    i64* used = cbuff_use_front(cb1);
    for(; used; used = cbuff_use_front(cb1), i++){
        CH_ASSERT(cb1->count == 2);
        CH_ASSERT(cb1->size == 5);
        CH_ASSERT(cb1->in_use == i + 1);
        CH_ASSERT(cb1->_add_next_index == 2);
        const ch_word use_next = (i + 1) % cb1->size;
        CH_ASSERT(cb1->_use_next_index == use_next );
        CH_ASSERT(cb1->_release_index == 0);
        CH_ASSERT(*used == vals[i]);
    }
    cbuff_delete(cb1);
    return result;

}



//Test pushing a small array and then using all things, then releasing
static ch_word test10()
{
    //Construct and check the basic conditions
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    //Now push a small array on
    ch_word len = 2;
    i64 vals[6] = {52,53,54,55,56,57};
    i64* val_p = cbuff_push_back_carray(cb1, vals, &len);
    CH_ASSERT(cb1->count == 2);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 2);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == vals[0]);
    CH_ASSERT(len == 2);


    //Now use everything
    int i = 0;
    i64* used = cbuff_use_front(cb1);
    ch_word use_next = 0;
    for(; used; used = cbuff_use_front(cb1), i++){
        CH_ASSERT(cb1->count == len);
        CH_ASSERT(cb1->size == 5);
        CH_ASSERT(cb1->in_use == i + 1);
        CH_ASSERT(cb1->_add_next_index == len);
        use_next = (i + 1) % cb1->size;
        CH_ASSERT(cb1->_use_next_index == use_next );
        CH_ASSERT(cb1->_release_index == 0);
        CH_ASSERT(*used == vals[i]);
    }
    CH_ASSERT( i =  1);
    CH_ASSERT(used == NULL);


    //Now pop everything off
    for(int i = 0; i < len; i++){
          cbuff_pop_front(cb1);
          CH_ASSERT(cb1->count == len - 1 - i);
          CH_ASSERT(cb1->size == 5 );
          CH_ASSERT(cb1->in_use == len - i - 1);
          CH_ASSERT(cb1->_add_next_index == len);
          CH_ASSERT(cb1->_use_next_index ==  use_next);
          const ch_word release_idx = (i + 1) % cb1->size;
          CH_ASSERT(cb1->_release_index == release_idx);
      }

    used = cbuff_use_front(cb1);
    CH_ASSERT(used == NULL);

    cbuff_delete(cb1);
    return result;

}




//Test pushing a bigger array and then usse some things, release and try to push and use more
static ch_word test11()
{
    //Construct and check the basic conditions
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    //Now push a small array on
    ch_word len = 3;
    i64 vals[6] = {52,53,54,55,56,57};
    i64* val_p = cbuff_push_back_carray(cb1, vals, &len);
    CH_ASSERT(cb1->count == 3);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 3);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == vals[0]);
    CH_ASSERT(len == 3);



    i64* used = cbuff_use_front(cb1);
    CH_ASSERT(used != NULL);
    CH_ASSERT(cb1->count == 3);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 1);
    CH_ASSERT(cb1->_add_next_index == 3);
    CH_ASSERT(cb1->_use_next_index == 1 );
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*used == vals[0]);


    cbuff_pop_front(cb1);
    CH_ASSERT(cb1->count == 2);
    CH_ASSERT(cb1->size == 5 );
    CH_ASSERT(cb1->in_use ==  0);
    CH_ASSERT(cb1->_add_next_index == 3);
    CH_ASSERT(cb1->_use_next_index ==  1);
    CH_ASSERT(cb1->_release_index == 1);

    cbuff_delete(cb1);
    return result;

}


//Test pushing a bigger array and then usse some things, release and try to push and use more
static ch_word test12()
{
    //Construct and check the basic conditions
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    //Now push a small array on
    ch_word len = 1;
    i64 vals[6] = {52,53,54,55,56,57};
    i64* val_p = cbuff_push_back_carray(cb1, vals, &len);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == vals[0]);
    CH_ASSERT(len == 1);

    //Use the first item
    i64* used = cbuff_use_front(cb1);
    CH_ASSERT(used != NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 1);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 1 );
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*used == vals[0]);

    //Then unuse it
    cbuff_unuse_front(cb1);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0 );
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*used == vals[0]);

    //Then use it again
    used = cbuff_use_front(cb1);
    CH_ASSERT(*used == vals[0]);
    CH_ASSERT(used != NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 1);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 1 );
    CH_ASSERT(cb1->_release_index == 0);

    //Now use another item
    used = cbuff_use_front(cb1);
    CH_ASSERT(used == NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 1);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 1 );
    CH_ASSERT(cb1->_release_index == 0);


    //Now we pop what we have used
    cbuff_pop_front(cb1);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5 );
    CH_ASSERT(cb1->in_use ==  0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index ==  1);
    CH_ASSERT(cb1->_release_index == 1);

    //Now we add some more
    len = 6;
    val_p = cbuff_push_back_carray(cb1, vals, &len);
    CH_ASSERT(len == 5);
    CH_ASSERT(cb1->count == 5);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 1);
    CH_ASSERT(cb1->_release_index == 1);
    CH_ASSERT(*val_p == vals[0]);

    //And try to use a value
    used = cbuff_use_front(cb1);
    CH_ASSERT(*used == vals[0]);
    CH_ASSERT(used != NULL);
    CH_ASSERT(cb1->count == 5);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 1);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 2 );
    CH_ASSERT(cb1->_release_index == 1);

    cbuff_delete(cb1);
    return result;

}


//Test pushing a bigger array and then usse some things, release and try to push and use more
static ch_word test13()
{
    //Construct and check the basic conditions
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);


    //Now push a small array on
    ch_word len = 1;
    i64 vals[6] = {52,53,54,55,56,57};
    i64* val_p = cbuff_push_back_carray(cb1, vals, &len);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val_p == vals[0]);
    CH_ASSERT(len == 1);

    //Pop that item off
    cbuff_pop_front(cb1);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 1);

    //Try to use an item
    i64* used = cbuff_use_front(cb1);
    CH_ASSERT(used == NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 1);

    i64 val2 = 8923;
    i64* val2_p = cbuff_push_back(cb1, &val2);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 2);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 1);
    CH_ASSERT(*val2_p == val2);
    CH_ASSERT(len == 1);

    //Try to use an item
    used = cbuff_use_front(cb1);
    CH_ASSERT(used != NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 1);
    CH_ASSERT(cb1->_add_next_index == 2);
    CH_ASSERT(cb1->_use_next_index == 2);
    CH_ASSERT(cb1->_release_index == 1);
    CH_ASSERT(*used = val2);

    cbuff_delete(cb1);
    return result;
}



//Test pushing a small array and then popping, then pushing big array and checking
static ch_word test14()
{
    ch_word result = 1;
    ch_cbuff_t* cb1 = ch_cbuff_new(5,sizeof(i64));

    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 0);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);

    ch_word len = 1;
    i64 vals1[6] = {52,53,54,55,56,57};
    i64* val1_p = cbuff_push_back_carray(cb1, vals1, &len);
    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 1);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 0);
    CH_ASSERT(*val1_p == vals1[0]);
    CH_ASSERT(len == 1);

    cbuff_pop_front(cb1);
    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 0);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 1);

    len = 5;
    i64 vals2[6] = {2,3,4,5,1};
    i64* val2_p = cbuff_push_back_carray(cb1, vals2, &len);
    CH_ASSERT(cb1->_array != NULL);
    CH_ASSERT(cb1->count == 5);
    CH_ASSERT(cb1->size == 5);
    CH_ASSERT(cb1->in_use == 0);
    CH_ASSERT(cb1->_add_next_index == 1);
    CH_ASSERT(cb1->_use_next_index == 0);
    CH_ASSERT(cb1->_release_index == 1);
    CH_ASSERT(len == 5);
    CH_ASSERT(*val2_p == vals2[0]);
    CH_ASSERT( ((i64*)(cb1->_array->first))[0] == vals2[4]);
    CH_ASSERT( ((i64*)(cb1->_array->first))[1] == vals2[0]);
    CH_ASSERT( ((i64*)(cb1->_array->first))[2] == vals2[1]);
    CH_ASSERT( ((i64*)(cb1->_array->first))[3] == vals2[2]);
    CH_ASSERT( ((i64*)(cb1->_array->first))[4] == vals2[3]);

    cbuff_delete(cb1);
    return result;

}



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    ch_word test_pass = 0;

    printf("CH Data Structures: Circular Queue Test 01: ");  printf("%s", (test_pass = test1()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 02: ");  printf("%s", (test_pass = test2()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 03: ");  printf("%s", (test_pass = test3()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 04: ");  printf("%s", (test_pass = test4()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 05: ");  printf("%s", (test_pass = test5()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 06: ");  printf("%s", (test_pass = test6()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 07: ");  printf("%s", (test_pass = test7()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 08: ");  printf("%s", (test_pass = test8()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 09: ");  printf("%s", (test_pass = test9()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 10: ");  printf("%s", (test_pass = test10()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 11: ");  printf("%s", (test_pass = test11()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 12: ");  printf("%s", (test_pass = test12()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 13: ");  printf("%s", (test_pass = test13()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    printf("CH Data Structures: Circular Queue Test 14: ");  printf("%s", (test_pass = test14()) ? "PASS\n" : "FAIL\n"); if(!test_pass) return 1;
    return 0;
}
