// CamIO 2: demo_array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../data_structs/linked_list/linked_list_std.h"
#include "../utils/util.h"

#include <stdio.h>

#define TYPE i64

#define NODE_DATAP(node) ( (void*)((u8*)node  + sizeof(void*) * 2) )
#define NODE_DATA(node)  ( (void*)((u8*)&node + sizeof(void*) * 2) )


void dump_list_TYPE(CH_LIST(i64)* ll)
{
    printf("---------------------------------------------------\n");
    printf("There are %li items in the list\n", ll->count);
    for(CH_LIST_IT(i64) i = ll->first(ll); i.value ;  ll->next(ll, &i)){
        printf("%li,", *(TYPE*)i.value);
    }
    printf("\n");
    printf("---------------------------------------------------\n");
}


static ch_word test1_TYPE(TYPE* test_data)
{
    ch_word result = 1;
    (void)test_data;

    //Make an empty list
    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));

    //Ensure that the basic invariants are met
    CH_ASSERT(ll1->first(ll1)._node == NULL);
    CH_ASSERT(ll1->first(ll1).value == NULL);
    CH_ASSERT(ll1->last(ll1)._node == NULL);
    CH_ASSERT(ll1->last(ll1).value == NULL);
    CH_ASSERT(ll1->end(ll1)._node == NULL);
    CH_ASSERT(ll1->end(ll1).value == NULL);
    CH_ASSERT(ll1->count == 0);


    CH_LIST(i64)* ll2 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));

    //Make sure the list is new
    CH_ASSERT(ll2 != ll1);

    //Check equality and free on an empty null list
    CH_ASSERT(ll1->eq(ll1,ll2));
    CH_ASSERT(ll2->eq(ll2,ll1));

    ll2->delete(ll2);
    ll1->delete(ll1);

    return result;
}


/* Insert an element into an empty list. Use both push back and push front*/
static ch_word test2_TYPE(TYPE* test_data)
{
    ch_word result = 1;

    //Make a new list
    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));

    //Push a value onto it
    CH_LIST_IT(i64) it1 = ll1->push_back(ll1, test_data[0]);

    //Test the the push returns a valid iterator
    CH_ASSERT( it1._node );
    CH_ASSERT( (*(TYPE*)NODE_DATAP(it1._node)) == test_data[0] );
    CH_ASSERT( it1.value );
    CH_ASSERT( *(TYPE*)it1.value == test_data[0] );

    //Test that first() returns a valid iterator
    CH_LIST_IT(i64) it2 = ll1->first(ll1);
    CH_ASSERT( it2._node );
    CH_ASSERT( (*(TYPE*)NODE_DATAP(it2._node)) == test_data[0] );
    CH_ASSERT( it2.value );
    CH_ASSERT( *(TYPE*)it2.value == test_data[0] );

    //Test that last() returns a valid iterator
    CH_LIST_IT(i64) it3 = ll1->last(ll1);
    CH_ASSERT( it3._node );
    CH_ASSERT( (*(TYPE*)NODE_DATAP(it1._node)) == test_data[0] );
    CH_ASSERT( it3.value );
    CH_ASSERT( *(TYPE*)it3.value == test_data[0] );


    //Test that first and last are equal
    CH_ASSERT( it2._node == it3._node);
    CH_ASSERT( it2.value == it3.value);

    //Create another list
    CH_LIST(i64)* ll2 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));

    //Make sure it is distinct
    CH_ASSERT(ll2 != ll1);

    //Check that the list is not equal to the original
    CH_ASSERT(!ll1->eq(ll1,ll2));

    //Push the same value onto the front
    CH_LIST_IT(i64) it4 = ll2->push_front(ll2, test_data[0]);

    //Test that the returned iterator is valid
    CH_ASSERT( it4._node );
    CH_ASSERT( (*(TYPE*)NODE_DATAP(it4._node)) == test_data[0] );
    CH_ASSERT( it4.value );
    CH_ASSERT( *(TYPE*)it4.value == test_data[0] );

    //Test that first() returns a valid iterator
    CH_LIST_IT(i64) it5 = ll2->first(ll2);
    CH_ASSERT( it5._node );
    CH_ASSERT( (*(TYPE*)NODE_DATAP(it5._node)) == test_data[0] );
    CH_ASSERT( it5.value );
    CH_ASSERT( *(TYPE*)it5.value == test_data[0] );

    //Test that last() returns a valid iterator
    CH_LIST_IT(i64) it7 = ll2->last(ll2);
    CH_ASSERT( it7._node );
    CH_ASSERT( (*(TYPE*)NODE_DATAP(it7._node)) == test_data[0] );
    CH_ASSERT( it7.value );
    CH_ASSERT( *(TYPE*)it7.value == test_data[0] );

    //Check that the lists are now equal
    CH_ASSERT(ll1->eq(ll1,ll2));
    CH_ASSERT(ll2->eq(ll2,ll1));

    //Clean up and bug out.
    ll1->delete(ll1);
    ll2->delete(ll2);
    return result;
}


/* Insert an element into an empty list. Use both push back and push front, then delete the elements*/
static ch_word test3_TYPE(TYPE* test_data)
{
    ch_word result = 1;

    //Make a new list and push a value onto it - We can assume this works from above
    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    ll1->push_back(ll1, test_data[0]);

    //Create another list and push a value onto it- We can assume this works from above
    CH_LIST(i64)* ll2 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    ll1->push_back(ll2, test_data[0]);

    CH_LIST_IT(i64) it3 = ll1->pop_front(ll1);

    //Ensure the list returns to base state
    CH_ASSERT(ll1->first(ll1)._node == NULL);
    CH_ASSERT(ll1->first(ll1).value == NULL);
    CH_ASSERT(ll1->last(ll1)._node == NULL);
    CH_ASSERT(ll1->last(ll1).value == NULL);
    CH_ASSERT(ll1->end(ll1)._node == NULL);
    CH_ASSERT(ll1->end(ll1).value == NULL);
    CH_ASSERT(ll1->count == 0);
    CH_ASSERT(it3._node == NULL);
    CH_ASSERT(it3.value == NULL);

    //Check inequality
    CH_ASSERT(!ll1->eq(ll1,ll2));


    //Remove element from the first list
    CH_LIST_IT(i64) it4 = ll2->pop_front(ll2);

    //Ensure the list returns to base state
    CH_ASSERT(ll2->first(ll2)._node == NULL);
    CH_ASSERT(ll2->first(ll2).value == NULL);
    CH_ASSERT(ll2->last(ll2)._node == NULL);
    CH_ASSERT(ll2->last(ll2).value == NULL);
    CH_ASSERT(ll2->end(ll2)._node == NULL);
    CH_ASSERT(ll2->end(ll2).value == NULL);
    CH_ASSERT(ll2->count == 0);


    CH_ASSERT(it4._node == NULL);
    CH_ASSERT(it4.value == NULL);

    //Check equality
    CH_ASSERT(ll1->eq(ll1,ll2));

    //Clean up and bug out.
    ll1->delete(ll1);
    ll2->delete(ll2);
    return result;
}


#define TEST_4_MAX 15

/* Insert all elements into an empty list. Use both push back, push front and push_back_carray*/
static ch_word test4_TYPE(TYPE* test_data)
{
    ch_word result = 1;

    //Make new lists
    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    CH_LIST(i64)* ll2 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    CH_LIST(i64)* ll3 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));

    for(ch_word i = 0; i < TEST_4_MAX; i++){
        //Push a value onto it
        CH_LIST_IT(i64) it1 = ll1->push_back(ll1, test_data[i]);

        CH_ASSERT(!ll1->eq(ll1,ll2));

        CH_LIST_IT(i64) it4 = ll2->push_front(ll2, test_data[TEST_4_MAX -1 - i]);

        //Test the the push returns a valid iterator
        CH_ASSERT( it1._node );
        CH_ASSERT( (*(TYPE*)NODE_DATAP(it1._node)) == test_data[i] );
        CH_ASSERT( it1.value );
        CH_ASSERT( *(TYPE*)it1.value == test_data[i] );

        //Test that first() returns a valid iterator
        CH_LIST_IT(i64) it2 = ll1->first(ll1);
        CH_ASSERT( it2._node );
        CH_ASSERT( (*(TYPE*)NODE_DATAP(it2._node)) == test_data[0] );
        CH_ASSERT( it2.value );
        CH_ASSERT( *(TYPE*)it2.value == test_data[0] );

        //Test that last() returns a valid iterator
        CH_LIST_IT(i64) it3 = ll1->last(ll1);
        CH_ASSERT( it3._node );
        CH_ASSERT( (*(TYPE*)NODE_DATAP(it1._node)) == test_data[i] );
        CH_ASSERT( it3.value );
        CH_ASSERT( *(TYPE*)it3.value == test_data[i] );

        //Test that the returned iterator is valid
        CH_ASSERT( it4._node );
        CH_ASSERT( (*(TYPE*)NODE_DATAP(it4._node)) == test_data[TEST_4_MAX -1 -i] );
        CH_ASSERT( it4.value );
        CH_ASSERT( *(TYPE*)it4.value == test_data[TEST_4_MAX -1 -i] );

        //Test that first() returns a valid iterator
        CH_LIST_IT(i64) it5 = ll2->first(ll2);
        CH_ASSERT( it5._node );
        CH_ASSERT( (*(TYPE*)NODE_DATAP(it5._node)) == test_data[TEST_4_MAX -1 - i] );
        CH_ASSERT( it5.value );
        CH_ASSERT( *(TYPE*)it5.value == test_data[TEST_4_MAX -1  -i] );

        //Test that last() returns a valid iterator
        CH_LIST_IT(i64) it7 = ll2->last(ll2);
        CH_ASSERT( it7._node );
        CH_ASSERT( (*(TYPE*)NODE_DATAP(it7._node)) == test_data[TEST_4_MAX -1] );
        CH_ASSERT( it7.value );
        CH_ASSERT( *(TYPE*)it7.value == test_data[TEST_4_MAX -1] );
    }

    CH_LIST_IT(i64) it8 = ll3->push_back_carray(ll3, test_data, TEST_4_MAX);
    //Test that the returned iterator is valid
    CH_ASSERT( it8._node );
    CH_ASSERT( (*(TYPE*)NODE_DATAP(it8._node)) == test_data[TEST_4_MAX -1] );
    CH_ASSERT( it8.value );
    CH_ASSERT( *(TYPE*)it8.value == test_data[TEST_4_MAX -1] );

    //Test that first() returns a valid iterator
    CH_LIST_IT(i64) it9 = ll2->first(ll2);
    CH_ASSERT( it9._node );
    CH_ASSERT( (*(TYPE*)NODE_DATAP(it9._node)) == test_data[0] );
    CH_ASSERT( it9.value );
    CH_ASSERT( *(TYPE*)it9.value == test_data[0] );

    //Test that last() returns a valid iterator
    CH_LIST_IT(i64) it10 = ll2->last(ll2);
    CH_ASSERT( it10._node );
    CH_ASSERT( (*(TYPE*)NODE_DATAP(it10._node)) == test_data[TEST_4_MAX -1] );
    CH_ASSERT( it10.value );
    CH_ASSERT( *(TYPE*)it10.value == test_data[TEST_4_MAX -1] );

    //Check that the lists are now equal
    CH_ASSERT(ll1->eq(ll1,ll2));
    CH_ASSERT(ll2->eq(ll2,ll1));
    CH_ASSERT(ll3->eq(ll3,ll1));
    CH_ASSERT(ll3->eq(ll3,ll2));
    CH_ASSERT(ll1->eq(ll1,ll3));
    CH_ASSERT(ll2->eq(ll2,ll3));

    //Clean up and bug out.
    ll1->delete(ll1);
    ll2->delete(ll2);
    ll3->delete(ll3);
    return result;
}


#define TEST_5_MAX 15

/* Insert all elements into an empty list. Use both push back, push front and push_back_carray
 * Then delete all elements from the front, back and middle.
 */
static ch_word test5_TYPE(TYPE* test_data)
{
    ch_word result = 1;

    //Make new lists
    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    CH_LIST(i64)* ll2 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    CH_LIST(i64)* ll3 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));

    for(ch_word i = 0; i < TEST_5_MAX; i++){
        ll1->push_back(ll1, test_data[i]);
        ll2->push_front(ll2, test_data[TEST_5_MAX -1 - i]);
    }
    ll3->push_back_carray(ll3, test_data, TEST_5_MAX);

    //Check that the lists are now equal
    CH_ASSERT(ll1->eq(ll1,ll2));
    CH_ASSERT(ll2->eq(ll2,ll1));
    CH_ASSERT(ll3->eq(ll3,ll1));
    CH_ASSERT(ll3->eq(ll3,ll2));
    CH_ASSERT(ll1->eq(ll1,ll3));
    CH_ASSERT(ll2->eq(ll2,ll3));

    for(ch_word i = 0; i < TEST_5_MAX + 10; i++){
        ll1->pop_back(ll1);
        ll2->pop_front(ll2);
        //dump_list_TYPE(ll2);
        //dump_list_TYPE(ll2);
    }

    CH_LIST_IT(i64) it1 = ll3->off(ll3,8);
    while( (it1 = ll3->remove_it(ll3,&it1) )._node ){
        //dump_list_TYPE(ll3);
    }

    CH_ASSERT( it1._node == NULL );
    CH_LIST(i64)* ll4 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    ll4->push_back_carray(ll4, test_data, 8);
    CH_ASSERT(ll4->eq(ll4,ll3));
    CH_ASSERT(ll3->eq(ll3,ll4));

    CH_LIST_IT(i64) it2 = ll3->first(ll3);
    while( (it2 = ll3->remove_it(ll3,&it2) )._node ){
        //dump_list_TYPE(ll3);
    }

    CH_ASSERT(ll1->eq(ll1,ll2));
    CH_ASSERT(ll2->eq(ll2,ll1));
    CH_ASSERT(ll3->eq(ll3,ll1));
    CH_ASSERT(ll3->eq(ll3,ll2));
    CH_ASSERT(ll1->eq(ll1,ll3));
    CH_ASSERT(ll2->eq(ll2,ll3));


    //Clean up and bug out.
    ll1->delete(ll1);
    ll2->delete(ll2);
    ll3->delete(ll3);
    return result;
}


//Try to find something in an empty list
static ch_word test6_TYPE(TYPE* test_data)
{
    ch_word result = 1;

    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));

    CH_LIST_IT(i64) start = ll1->first(ll1);
    CH_LIST_IT(i64) end = ll1->end(ll1);
    CH_ASSERT(ll1->find(ll1, &start, &end , test_data[1])._node == NULL);
    CH_ASSERT(ll1->find(ll1, &start, &end , test_data[1]).value == NULL);

    ll1->delete(ll1);

    return result;
}


static ch_word test7_TYPE(TYPE* test_data)
{
    ch_word result = 1;

    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    ll1->push_back_carray(ll1, test_data, 3);

    CH_LIST_IT(i64) start = ll1->first(ll1);
    CH_LIST_IT(i64) end = ll1->end(ll1);
    CH_LIST_IT(i64) it = ll1->find(ll1, &start, &end , test_data[1]);
    CH_ASSERT(it._node);
    CH_ASSERT(it.value);
    CH_ASSERT(*(TYPE*)it.value == test_data[1]);

    ll1->delete(ll1);

    return result;
}


static ch_word test8_TYPE(TYPE* test_data)
{
    ch_word result = 1;

    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    ll1->push_back_carray(ll1, test_data, 3);

    CH_LIST_IT(i64) start = ll1->first(ll1);
    CH_LIST_IT(i64) end = ll1->end(ll1);
    CH_LIST_IT(i64) start1 = start; ll1->forward(ll1,&start1,1);
    CH_LIST_IT(i64) start2 = start; ll1->forward(ll1,&start2,2);

    TYPE value = 42;
    CH_ASSERT(ll1->find(ll1,&start, &end, value)._node == NULL);
    CH_ASSERT(ll1->find(ll1,&start, &end, value).value == NULL);

    CH_ASSERT(ll1->find(ll1,&end, &end, value)._node == NULL);
    CH_ASSERT(ll1->find(ll1,&end, &end, value).value == NULL);

    CH_ASSERT(ll1->find(ll1,&start, &start1, test_data[1])._node == NULL);
    CH_ASSERT(ll1->find(ll1,&start, &start1, test_data[1]).value == NULL);

    CH_ASSERT(ll1->find(ll1,&start2, &start2, test_data[2])._node == NULL);
    CH_ASSERT(ll1->find(ll1,&start2, &start2, test_data[2]).value == NULL);

    CH_ASSERT(ll1->find(ll1,&start, &start2, test_data[1])._node != NULL);
    CH_ASSERT(ll1->find(ll1,&start, &start2, test_data[1]).value != NULL);
    CH_ASSERT(*(TYPE*)ll1->find(ll1,&start, &start2, test_data[1]).value == test_data[1]);

    ll1->delete(ll1);

    return result;
}


//Test the two types of remove operations
static ch_word test9_TYPE()
{
    ch_word result = 1;
    //This is the data we'll use to test on
    TYPE test_array1[15] = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};
    TYPE test_array2[14] = {8,5,3,4,6,7,9,7,1,6,1,0,1,6};
    TYPE test_array3[11] = {8,5,3,4,6,7,9,7,6,0,6};


    //Make for linked lists, the first two use dataset 1, the second two are the expected results
    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    CH_LIST(i64)* ll2 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    CH_LIST(i64)* ll3 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    CH_LIST(i64)* ll4 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));

    //Populate all lists
    ll1->push_back_carray(ll1, test_array1, 15);
    ll2->push_back_carray(ll2, test_array1, 15);
    ll3->push_back_carray(ll3, test_array2, 14);
    ll4->push_back_carray(ll4, test_array3, 11);

    //Check that everything has populated as we expect. L1 and L2 should be equal, everything else should not be
    CH_ASSERT(ll1->eq(ll1,ll2));
    CH_ASSERT(!ll1->eq(ll1,ll3));
    CH_ASSERT(!ll1->eq(ll1,ll4));
    CH_ASSERT(ll2->eq(ll2,ll1));
    CH_ASSERT(!ll2->eq(ll2,ll3));
    CH_ASSERT(!ll2->eq(ll2,ll4));
    CH_ASSERT(!ll3->eq(ll3,ll1));
    CH_ASSERT(!ll3->eq(ll3,ll2));
    CH_ASSERT(!ll3->eq(ll3,ll4));
    CH_ASSERT(!ll4->eq(ll4,ll1));
    CH_ASSERT(!ll4->eq(ll4,ll2));
    CH_ASSERT(!ll4->eq(ll4,ll3));

    //Apply the find and delete method
    CH_LIST_IT(i64) start   = ll1->first(ll1);
    CH_LIST_IT(i64) end     = ll1->end(ll1);
    CH_LIST_IT(i64) found   = ll1->find(ll1,&start,&end,1);
    CH_ASSERT(found.value);
    CH_ASSERT(*found.value == 1);
    CH_LIST_IT(i64) next    = ll1->remove_it(ll1,&found);
    CH_ASSERT(next.value);
    CH_ASSERT(*next.value == 3);
    CH_ASSERT(ll1->eq(ll1,ll3));

    //Apply the delete all method
    ll2->remove_all(ll2,1);
    CH_ASSERT(ll2->eq(ll2,ll4));


    //Now re-check that we've established the state we wanted
    CH_ASSERT(!ll1->eq(ll1,ll2));
    CH_ASSERT(ll1->eq(ll1,ll3));
    CH_ASSERT(!ll1->eq(ll1,ll4));
    CH_ASSERT(!ll2->eq(ll2,ll1));
    CH_ASSERT(!ll2->eq(ll2,ll3));
    CH_ASSERT(ll2->eq(ll2,ll4));
    CH_ASSERT(ll3->eq(ll3,ll1));
    CH_ASSERT(!ll3->eq(ll3,ll2));
    CH_ASSERT(!ll3->eq(ll3,ll4));
    CH_ASSERT(!ll4->eq(ll4,ll1));
    CH_ASSERT(ll4->eq(ll4,ll2));
    CH_ASSERT(!ll4->eq(ll4,ll3));


    ll1->delete(ll1);
    ll2->delete(ll2);
    ll3->delete(ll3);
    ll4->delete(ll4);

    return result;
}

//Test the ordered insert function
static ch_word test10_TYPE(TYPE* test_data)
{
    ch_word result = 1;
    //This is the data we'll use to test on
    TYPE result_array[15] = {0,1,1,1,1,3,4,5,6,6,6,7,7,8,9};


    CH_LIST(i64)* ll1 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    CH_LIST(i64)* ll2 = CH_LIST_NEW(i64,CH_LIST_CMP(i64));
    ll1->insert_carray_ordered(ll1,test_data,15);
    ll2->push_back_carray(ll2,result_array,15);
    CH_ASSERT(ll1->eq(ll1,ll2));
    CH_ASSERT(ll2->eq(ll2,ll1));

    return result;

}



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    TYPE test_array[15] = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};
    //ch_word test_array_sorted[150] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9};

    ch_word test_result = 0;

    printf("CH Data Structures: Typed Linked List Test 01: ");  printf("%s", (test_result = test1_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Typed Linked List Test 02: ");  printf("%s", (test_result = test2_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Typed Linked List Test 03: ");  printf("%s", (test_result = test3_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Typed Linked List Test 04: ");  printf("%s", (test_result = test4_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Typed Linked List Test 05: ");  printf("%s", (test_result = test5_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Typed Linked List Test 06: ");  printf("%s", (test_result = test6_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Typed Linked List Test 07: ");  printf("%s", (test_result = test7_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Typed Linked List Test 08: ");  printf("%s", (test_result = test8_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Typed Linked List Test 09: ");  printf("%s", (test_result = test9_TYPE()) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Typed Linked List Test 10: ");  printf("%s", (test_result = test10_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Typed Linked List Test 11: ");  printf("%s", (test_result = test11_TYPE(test_array, test_array_sorted)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Typed Linked List Test 12: ");  printf("%s", (test_result = test12_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Typed Linked List Test 13: ");  printf("%s", (test_result = test13_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Typed Linked List Test 14: ");  printf("%s", (test_result = test14_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Typed Linked List Test 15: ");  printf("%s", (test_result = test16_TYPE(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;

    return 0;
}



