// CamIO 2: demo_array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../data_structs/linked_list/linked_list.h"
#include "../utils/util.h"

#include <stdio.h>


#define NODE_DATAP(node) ( (void*)((u8*)node  + sizeof(ch_llist_node_t)) )
#define NODE_DATA(node)  ( (void*)((u8*)&node + sizeof(ch_llist_node_t)) )


static ch_word cmp_i64(void* lhs, void* rhs)
{
    if( *(i64*)lhs < *(i64*)rhs ){
        return -1;
    }

    if( *(i64*)lhs > *(i64*)rhs ){
        return 1;
    }

    return 0;
}


void dump_list_i64(ch_llist_t* ll)
{
    printf("---------------------------------------------------\n");
    printf("There are %li items in the list\n", ll->count);
    for(ch_llist_it i = llist_first(ll); i.value ;  llist_next(ll, &i)){
        printf("%li,", *(i64*)i.value);
    }
    printf("\n");
    printf("---------------------------------------------------\n");
}


static ch_word test1_i64(i64* test_data)
{
    ch_word result = 1;
    (void)test_data;

    //Make an empty list
    ch_llist_t* ll1 = ch_llist_new(sizeof(i64),cmp_i64);

    //Ensure that the basic invariants are met
    CH_ASSERT(ll1->_element_size == sizeof(i64));
    CH_ASSERT(ll1->_first == NULL);
    CH_ASSERT(ll1->_last == NULL);
    CH_ASSERT(llist_first(ll1)._node == NULL);
    CH_ASSERT(llist_first(ll1).value == NULL);
    CH_ASSERT(llist_last(ll1)._node == NULL);
    CH_ASSERT(llist_last(ll1).value == NULL);
    CH_ASSERT(llist_end(ll1)._node == NULL);
    CH_ASSERT(llist_end(ll1).value == NULL);
    CH_ASSERT(ll1->count == 0);
    CH_ASSERT(ll1->_cmp == (cmp_void_f)cmp_i64);


    //Make an empty list that will fail.
    printf("\n------------------------------------------------\n");
    printf("Expected Output:\n");
    printf("Error: invalid element size (<=0), must have *some* data\n");
    printf("------------------------------------------------\n");
    printf("Actual Output:\n");
    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
    printf("------------------------------------------------\n");
    printf("CH Data Structures: Linked List Test 02: ");

    //Make sure the list is new
    CH_ASSERT(ll2 != ll1);

    //Check inequality and free on an empty null list
    CH_ASSERT(!llist_eq(ll1,ll2));
    CH_ASSERT(!llist_eq(ll2,ll1));
    llist_delete(ll2);


    //Make an empty list that will succeed
    ch_llist_t* ll3 = ch_llist_new(sizeof(i64),cmp_i64);

    //Make sure the list is new
    CH_ASSERT(ll3 != ll1);

    //Check equality
    CH_ASSERT(llist_eq(ll1,ll3));
    CH_ASSERT(llist_eq(ll3,ll1));

    llist_delete(ll3);
    llist_delete(ll1);



    return result;
}


/* Insert an element into an empty list. Use both push back and push front*/
static ch_word test2_i64(i64* test_data)
{
    ch_word result = 1;

    //Make a new list
    ch_llist_t* ll1 = ch_llist_new(sizeof(i64),cmp_i64);

    //Push a value onto it
    ch_llist_it it1 = llist_push_back(ll1, &test_data[0]);

    //Test the the push returns a valid iterator
    CH_ASSERT( it1._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it1._node)) == test_data[0] );
    CH_ASSERT( it1.value );
    CH_ASSERT( *(i64*)it1.value == test_data[0] );

    //Test that first() returns a valid iterator
    ch_llist_it it2 = llist_first(ll1);
    CH_ASSERT( it2._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it2._node)) == test_data[0] );
    CH_ASSERT( it2.value );
    CH_ASSERT( *(i64*)it2.value == test_data[0] );

    //Test that last() returns a valid iterator
    ch_llist_it it3 = llist_last(ll1);
    CH_ASSERT( it3._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it1._node)) == test_data[0] );
    CH_ASSERT( it3.value );
    CH_ASSERT( *(i64*)it3.value == test_data[0] );


    //Test that first and last are equal
    CH_ASSERT( it2._node == it3._node);
    CH_ASSERT( it2.value == it3.value);

    //Create another list
    ch_llist_t* ll2 = ch_llist_new(sizeof(i64),cmp_i64);

    //Make sure it is distinct
    CH_ASSERT(ll2 != ll1);

    //Check that the list is not equal to the original
    CH_ASSERT(!llist_eq(ll1,ll2));

    //Push the same value onto the front
    ch_llist_it it4 = llist_push_front(ll2, &test_data[0]);

    //Test that the returned iterator is valid
    CH_ASSERT( it4._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it4._node)) == test_data[0] );
    CH_ASSERT( it4.value );
    CH_ASSERT( *(i64*)it4.value == test_data[0] );

    //Test that first() returns a valid iterator
    ch_llist_it it5 = llist_first(ll2);
    CH_ASSERT( it5._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it5._node)) == test_data[0] );
    CH_ASSERT( it5.value );
    CH_ASSERT( *(i64*)it5.value == test_data[0] );

    //Test that last() returns a valid iterator
    ch_llist_it it7 = llist_last(ll2);
    CH_ASSERT( it7._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it7._node)) == test_data[0] );
    CH_ASSERT( it7.value );
    CH_ASSERT( *(i64*)it7.value == test_data[0] );

    //Check that the lists are now equal
    CH_ASSERT(llist_eq(ll1,ll2));
    CH_ASSERT(llist_eq(ll2,ll1));

    //Clean up and bug out.
    llist_delete(ll1);
    llist_delete(ll2);
    return result;
}


/* Insert an element into an empty list. Use both push back and push front, then delete the elements*/
static ch_word test3_i64(i64* test_data)
{
    ch_word result = 1;

    //Make a new list and push a value onto it - We can assume this works from above
    ch_llist_t* ll1 = ch_llist_new(sizeof(i64),cmp_i64);
    llist_push_back(ll1, &test_data[0]);

    //Create another list and push a value onto it- We can assume this works from above
    ch_llist_t* ll2 = ch_llist_new(sizeof(i64),cmp_i64);
    llist_push_back(ll2, &test_data[0]);

    ch_llist_it it3 = llist_pop_front(ll1);

    //Ensure the list returns to base state
    CH_ASSERT(ll1->_element_size == sizeof(i64));
    CH_ASSERT(ll1->_first == NULL);
    CH_ASSERT(ll1->_last == NULL);
    CH_ASSERT(llist_first(ll1)._node == NULL);
    CH_ASSERT(llist_first(ll1).value == NULL);
    CH_ASSERT(llist_last(ll1)._node == NULL);
    CH_ASSERT(llist_last(ll1).value == NULL);
    CH_ASSERT(llist_end(ll1)._node == NULL);
    CH_ASSERT(llist_end(ll1).value == NULL);
    CH_ASSERT(ll1->count == 0);
    CH_ASSERT(ll1->_cmp == (cmp_void_f)cmp_i64);
    CH_ASSERT(it3._node == NULL);
    CH_ASSERT(it3.value == NULL);

    //Check inequality
    CH_ASSERT(!llist_eq(ll1,ll2));


    //Remove element from the first list
    ch_llist_it it4 = llist_pop_front(ll2);

    //Ensure the list returns to base state
    CH_ASSERT(ll2->_element_size == sizeof(i64));
    CH_ASSERT(ll2->_first == NULL);
    CH_ASSERT(ll2->_last == NULL);
    CH_ASSERT(llist_first(ll2)._node == NULL);
    CH_ASSERT(llist_first(ll2).value == NULL);
    CH_ASSERT(llist_last(ll2)._node == NULL);
    CH_ASSERT(llist_last(ll2).value == NULL);
    CH_ASSERT(llist_end(ll2)._node == NULL);
    CH_ASSERT(llist_end(ll2).value == NULL);
    CH_ASSERT(ll2->count == 0);
    CH_ASSERT(ll2->_cmp == (cmp_void_f)cmp_i64);

    CH_ASSERT(it4._node == NULL);
    CH_ASSERT(it4.value == NULL);

    //Check equality
    CH_ASSERT(llist_eq(ll1,ll2));

    //Clean up and bug out.
    llist_delete(ll1);
    llist_delete(ll2);
    return result;
}


#define TEST_4_MAX 15

/* Insert all elements into an empty list. Use both push back, push front and push_back_carray*/
static ch_word test4_i64(i64* test_data)
{
    ch_word result = 1;

    //Make new lists
    ch_llist_t* ll1 = ch_llist_new(sizeof(i64),cmp_i64);
    ch_llist_t* ll2 = ch_llist_new(sizeof(i64),cmp_i64);
    ch_llist_t* ll3 = ch_llist_new(sizeof(i64),cmp_i64);

    for(ch_word i = 0; i < TEST_4_MAX; i++){
        //Push a value onto it
        ch_llist_it it1 = llist_push_back(ll1, &test_data[i]);

        CH_ASSERT(!llist_eq(ll1,ll2));

        ch_llist_it it4 = llist_push_front(ll2, &test_data[TEST_4_MAX -1 - i]);

        //Test the the push returns a valid iterator
        CH_ASSERT( it1._node );
        CH_ASSERT( (*(i64*)NODE_DATAP(it1._node)) == test_data[i] );
        CH_ASSERT( it1.value );
        CH_ASSERT( *(i64*)it1.value == test_data[i] );

        //Test that first() returns a valid iterator
        ch_llist_it it2 = llist_first(ll1);
        CH_ASSERT( it2._node );
        CH_ASSERT( (*(i64*)NODE_DATAP(it2._node)) == test_data[0] );
        CH_ASSERT( it2.value );
        CH_ASSERT( *(i64*)it2.value == test_data[0] );

        //Test that last() returns a valid iterator
        ch_llist_it it3 = llist_last(ll1);
        CH_ASSERT( it3._node );
        CH_ASSERT( (*(i64*)NODE_DATAP(it1._node)) == test_data[i] );
        CH_ASSERT( it3.value );
        CH_ASSERT( *(i64*)it3.value == test_data[i] );

        //Test that the returned iterator is valid
        CH_ASSERT( it4._node );
        CH_ASSERT( (*(i64*)NODE_DATAP(it4._node)) == test_data[TEST_4_MAX -1 -i] );
        CH_ASSERT( it4.value );
        CH_ASSERT( *(i64*)it4.value == test_data[TEST_4_MAX -1 -i] );

        //Test that first() returns a valid iterator
        ch_llist_it it5 = llist_first(ll2);
        CH_ASSERT( it5._node );
        CH_ASSERT( (*(i64*)NODE_DATAP(it5._node)) == test_data[TEST_4_MAX -1 - i] );
        CH_ASSERT( it5.value );
        CH_ASSERT( *(i64*)it5.value == test_data[TEST_4_MAX -1  -i] );

        //Test that last() returns a valid iterator
        ch_llist_it it7 = llist_last(ll2);
        CH_ASSERT( it7._node );
        CH_ASSERT( (*(i64*)NODE_DATAP(it7._node)) == test_data[TEST_4_MAX -1] );
        CH_ASSERT( it7.value );
        CH_ASSERT( *(i64*)it7.value == test_data[TEST_4_MAX -1] );
    }

    ch_llist_it it8 = llist_push_back_carray(ll3, test_data, TEST_4_MAX);
    //Test that the returned iterator is valid
    CH_ASSERT( it8._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it8._node)) == test_data[TEST_4_MAX -1] );
    CH_ASSERT( it8.value );
    CH_ASSERT( *(i64*)it8.value == test_data[TEST_4_MAX -1] );

    //Test that first() returns a valid iterator
    ch_llist_it it9 = llist_first(ll2);
    CH_ASSERT( it9._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it9._node)) == test_data[0] );
    CH_ASSERT( it9.value );
    CH_ASSERT( *(i64*)it9.value == test_data[0] );

    //Test that last() returns a valid iterator
    ch_llist_it it10 = llist_last(ll2);
    CH_ASSERT( it10._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it10._node)) == test_data[TEST_4_MAX -1] );
    CH_ASSERT( it10.value );
    CH_ASSERT( *(i64*)it10.value == test_data[TEST_4_MAX -1] );

    //Check that the lists are now equal
    CH_ASSERT(llist_eq(ll1,ll2));
    CH_ASSERT(llist_eq(ll2,ll1));
    CH_ASSERT(llist_eq(ll3,ll1));
    CH_ASSERT(llist_eq(ll3,ll2));
    CH_ASSERT(llist_eq(ll1,ll3));
    CH_ASSERT(llist_eq(ll2,ll3));

    //Clean up and bug out.
    llist_delete(ll1);
    llist_delete(ll2);
    llist_delete(ll3);
    return result;
}


#define TEST_5_MAX 15

/* Insert all elements into an empty list. Use both push back, push front and push_back_carray
 * Then delete all elements from the front, back and middle.
 */
static ch_word test5_i64(i64* test_data)
{
    ch_word result = 1;

    //Make new lists
    ch_llist_t* ll1 = ch_llist_new(sizeof(i64),cmp_i64);
    ch_llist_t* ll2 = ch_llist_new(sizeof(i64),cmp_i64);
    ch_llist_t* ll3 = ch_llist_new(sizeof(i64),cmp_i64);

    for(ch_word i = 0; i < TEST_5_MAX; i++){
        llist_push_back(ll1, &test_data[i]);
        llist_push_front(ll2, &test_data[TEST_5_MAX -1 - i]);
    }
    llist_push_back_carray(ll3, test_data, TEST_5_MAX);

    //Check that the lists are now equal
    CH_ASSERT(llist_eq(ll1,ll2));
    CH_ASSERT(llist_eq(ll2,ll1));
    CH_ASSERT(llist_eq(ll3,ll1));
    CH_ASSERT(llist_eq(ll3,ll2));
    CH_ASSERT(llist_eq(ll1,ll3));
    CH_ASSERT(llist_eq(ll2,ll3));

    for(ch_word i = 0; i < TEST_5_MAX + 10; i++){
        llist_pop_back(ll1);
        llist_pop_front(ll2);
        //dump_list_i64(ll2);
        //dump_list_i64(ll2);
    }

    ch_llist_it it1 = llist_off(ll3,8);
    while( (it1 = llist_remove(ll3,&it1) )._node ){
        //dump_list_i64(ll3);
    }

    CH_ASSERT( it1._node == NULL );
    ch_llist_t* ll4 = ch_llist_new(sizeof(i64),cmp_i64);
    llist_push_back_carray(ll4, test_data, 8);
    CH_ASSERT(llist_eq(ll4,ll3));
    CH_ASSERT(llist_eq(ll3,ll4));

    ch_llist_it it2 = llist_first(ll3);
    while( (it2 = llist_remove(ll3,&it2) )._node ){
        //dump_list_i64(ll3);
    }

    CH_ASSERT(llist_eq(ll1,ll2));
    CH_ASSERT(llist_eq(ll2,ll1));
    CH_ASSERT(llist_eq(ll3,ll1));
    CH_ASSERT(llist_eq(ll3,ll2));
    CH_ASSERT(llist_eq(ll1,ll3));
    CH_ASSERT(llist_eq(ll2,ll3));


    //Clean up and bug out.
    llist_delete(ll1);
    llist_delete(ll2);
    llist_delete(ll3);
    return result;
}


//Try to find something in an empty list
static ch_word test6_i64(i64* test_data)
{
    ch_word result = 1;

    ch_llist_t* ll1 = ch_llist_new(sizeof(i64),cmp_i64);

    ch_llist_it start = llist_first(ll1);
    ch_llist_it end = llist_end(ll1);
    CH_ASSERT(llist_find(ll1, &start, &end , &test_data[1])._node == NULL);
    CH_ASSERT(llist_find(ll1, &start, &end , &test_data[1]).value == NULL);

    llist_delete(ll1);

    return result;
}


static ch_word test7_i64(i64* test_data)
{
    ch_word result = 1;

    ch_llist_t* ll1 = ch_llist_new(sizeof(i64),cmp_i64);
    llist_push_back_carray(ll1, test_data, 3);

    ch_llist_it start = llist_first(ll1);
    ch_llist_it end = llist_end(ll1);
    ch_llist_it it = llist_find(ll1, &start, &end , &test_data[1]);
    CH_ASSERT(it._node);
    CH_ASSERT(it.value);
    CH_ASSERT(*(i64*)it.value == test_data[1]);

    llist_delete(ll1);

    return result;
}


static ch_word test8_i64(i64* test_data)
{
    ch_word result = 1;

    ch_llist_t* ll1 = ch_llist_new(sizeof(i64),cmp_i64);
    llist_push_back_carray(ll1, test_data, 3);

    ch_llist_it start = llist_first(ll1);
    ch_llist_it end = llist_end(ll1);
    ch_llist_it start1 = start; llist_forward(ll1,&start1,1);
    ch_llist_it start2 = start; llist_forward(ll1,&start2,2);

    i64 value = 42;
    CH_ASSERT(llist_find(ll1,&start, &end, &value)._node == NULL);
    CH_ASSERT(llist_find(ll1,&start, &end, &value).value == NULL);

    CH_ASSERT(llist_find(ll1,&end, &end, &value)._node == NULL);
    CH_ASSERT(llist_find(ll1,&end, &end, &value).value == NULL);

    CH_ASSERT(llist_find(ll1,&start, &start1, &test_data[1])._node == NULL);
    CH_ASSERT(llist_find(ll1,&start, &start1, &test_data[1]).value == NULL);

    CH_ASSERT(llist_find(ll1,&start2, &start2, &test_data[2])._node == NULL);
    CH_ASSERT(llist_find(ll1,&start2, &start2, &test_data[2]).value == NULL);

    CH_ASSERT(llist_find(ll1,&start, &start2, &test_data[1])._node != NULL);
    CH_ASSERT(llist_find(ll1,&start, &start2, &test_data[1]).value != NULL);
    CH_ASSERT(*(i64*)llist_find(ll1,&start, &start2, &test_data[1]).value == test_data[1]);

    llist_delete(ll1);

    return result;
}



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    i64 test_array[15] = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};
    //ch_word test_array_sorted[150] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9};

    ch_word test_result = 0;

    printf("CH Data Structures: Generic Linked List Test 01: ");  printf("%s", (test_result = test1_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Linked List Test 02: ");  printf("%s", (test_result = test2_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Linked List Test 03: ");  printf("%s", (test_result = test3_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Linked List Test 04: ");  printf("%s", (test_result = test4_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Linked List Test 05: ");  printf("%s", (test_result = test5_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Linked List Test 06: ");  printf("%s", (test_result = test6_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Linked List Test 07: ");  printf("%s", (test_result = test7_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Linked List Test 08: ");  printf("%s", (test_result = test8_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 09: ");  printf("%s", (test_result = test9_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 10: ");  printf("%s", (test_result = test10_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 11: ");  printf("%s", (test_result = test11_i64(test_array, test_array_sorted)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 12: ");  printf("%s", (test_result = test12_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 13: ");  printf("%s", (test_result = test13_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 14: ");  printf("%s", (test_result = test14_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 15: ");  printf("%s", (test_result = test16_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;

    return 0;
}



