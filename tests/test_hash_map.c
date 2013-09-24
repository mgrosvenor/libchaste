// CamIO 2: demo_array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../data_structs/hash_map/hash_map.h"
#include "../utils/util.h"

#include <stdio.h>


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


//void dump_hash_map_i64(ch_hash_map* ll)
//{
//    printf("---------------------------------------------------\n");
//    printf("There are %li items in the hash_map\n", ll->count);
//    for(ch_hash_map_it i = hash_map_first(ll); i.value ;  hash_map_next(ll, &i)){
//        printf("%li,", *(i64*)i.value);
//    }
//    printf("\n");
//    printf("---------------------------------------------------\n");
//}


static ch_word test1_i64(i64* test_data)
{
    ch_word result = 1;
    (void)test_data;

    //Make an empty hash_map
    ch_hash_map* hm1 = ch_hash_map_new(10,sizeof(void*),cmp_i64);
    hash_map_delete(hm1);

    return result;
}

typedef struct {
    char* key;
    char* value;
} kv;


static ch_word test2_i64(i64* test_data)
{
    char* [2]


    ch_word result = 1;
    (void)test_data;

    //Make an empty hash_map
    ch_hash_map* hm1 = ch_hash_map_new(10,sizeof(void*),cmp_i64);

    hash_map_push_unsafe(hm1,"test1",strlen("test1"), )

    hash_map_delete(hm1);

    return result;
}



///* Insert an element into an empty hash_map. Use both push back and push front*/
//static ch_word test2_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    //Make a new hash_map
//    ch_hash_map* ll1 = ch_hash_map_new(sizeof(i64),cmp_i64);
//
//    //Push a value onto it
//    ch_hash_map_it it1 = hash_map_push_back(ll1, &test_data[0]);
//
//    //Test the the push returns a valid iterator
//    CH_ASSERT( it1._node );
//    CH_ASSERT( (*(i64*)NODE_DATAP(it1._node)) == test_data[0] );
//    CH_ASSERT( it1.value );
//    CH_ASSERT( *(i64*)it1.value == test_data[0] );
//
//    //Test that first() returns a valid iterator
//    ch_hash_map_it it2 = hash_map_first(ll1);
//    CH_ASSERT( it2._node );
//    CH_ASSERT( (*(i64*)NODE_DATAP(it2._node)) == test_data[0] );
//    CH_ASSERT( it2.value );
//    CH_ASSERT( *(i64*)it2.value == test_data[0] );
//
//    //Test that last() returns a valid iterator
//    ch_hash_map_it it3 = hash_map_last(ll1);
//    CH_ASSERT( it3._node );
//    CH_ASSERT( (*(i64*)NODE_DATAP(it1._node)) == test_data[0] );
//    CH_ASSERT( it3.value );
//    CH_ASSERT( *(i64*)it3.value == test_data[0] );
//
//
//    //Test that first and last are equal
//    CH_ASSERT( it2._node == it3._node);
//    CH_ASSERT( it2.value == it3.value);
//
//    //Create another hash_map
//    ch_hash_map* ll2 = ch_hash_map_new(sizeof(i64),cmp_i64);
//
//    //Make sure it is distinct
//    CH_ASSERT(ll2 != ll1);
//
//    //Check that the hash_map is not equal to the original
//    CH_ASSERT(!hash_map_eq(ll1,ll2));
//
//    //Push the same value onto the front
//    ch_hash_map_it it4 = hash_map_push_front(ll2, &test_data[0]);
//
//    //Test that the returned iterator is valid
//    CH_ASSERT( it4._node );
//    CH_ASSERT( (*(i64*)NODE_DATAP(it4._node)) == test_data[0] );
//    CH_ASSERT( it4.value );
//    CH_ASSERT( *(i64*)it4.value == test_data[0] );
//
//    //Test that first() returns a valid iterator
//    ch_hash_map_it it5 = hash_map_first(ll2);
//    CH_ASSERT( it5._node );
//    CH_ASSERT( (*(i64*)NODE_DATAP(it5._node)) == test_data[0] );
//    CH_ASSERT( it5.value );
//    CH_ASSERT( *(i64*)it5.value == test_data[0] );
//
//    //Test that last() returns a valid iterator
//    ch_hash_map_it it7 = hash_map_last(ll2);
//    CH_ASSERT( it7._node );
//    CH_ASSERT( (*(i64*)NODE_DATAP(it7._node)) == test_data[0] );
//    CH_ASSERT( it7.value );
//    CH_ASSERT( *(i64*)it7.value == test_data[0] );
//
//    //Check that the hash_maps are now equal
//    CH_ASSERT(hash_map_eq(ll1,ll2));
//    CH_ASSERT(hash_map_eq(ll2,ll1));
//
//    //Clean up and bug out.
//    hash_map_delete(ll1);
//    hash_map_delete(ll2);
//    return result;
//}
//
//
///* Insert an element into an empty hash_map. Use both push back and push front, then delete the elements*/
//static ch_word test3_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    //Make a new hash_map and push a value onto it - We can assume this works from above
//    ch_hash_map* ll1 = ch_hash_map_new(sizeof(i64),cmp_i64);
//    hash_map_push_back(ll1, &test_data[0]);
//
//    //Create another hash_map and push a value onto it- We can assume this works from above
//    ch_hash_map* ll2 = ch_hash_map_new(sizeof(i64),cmp_i64);
//    hash_map_push_back(ll2, &test_data[0]);
//
//    ch_hash_map_it it3 = hash_map_pop_front(ll1);
//
//    //Ensure the hash_map returns to base state
//    CH_ASSERT(ll1->_element_size == sizeof(i64));
//    CH_ASSERT(ll1->_first == NULL);
//    CH_ASSERT(ll1->_last == NULL);
//    CH_ASSERT(hash_map_first(ll1)._node == NULL);
//    CH_ASSERT(hash_map_first(ll1).value == NULL);
//    CH_ASSERT(hash_map_last(ll1)._node == NULL);
//    CH_ASSERT(hash_map_last(ll1).value == NULL);
//    CH_ASSERT(hash_map_end(ll1)._node == NULL);
//    CH_ASSERT(hash_map_end(ll1).value == NULL);
//    CH_ASSERT(ll1->count == 0);
//    CH_ASSERT(ll1->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(it3._node == NULL);
//    CH_ASSERT(it3.value == NULL);
//
//    //Check inequality
//    CH_ASSERT(!hash_map_eq(ll1,ll2));
//
//
//    //Remove element from the first hash_map
//    ch_hash_map_it it4 = hash_map_pop_front(ll2);
//
//    //Ensure the hash_map returns to base state
//    CH_ASSERT(ll2->_element_size == sizeof(i64));
//    CH_ASSERT(ll2->_first == NULL);
//    CH_ASSERT(ll2->_last == NULL);
//    CH_ASSERT(hash_map_first(ll2)._node == NULL);
//    CH_ASSERT(hash_map_first(ll2).value == NULL);
//    CH_ASSERT(hash_map_last(ll2)._node == NULL);
//    CH_ASSERT(hash_map_last(ll2).value == NULL);
//    CH_ASSERT(hash_map_end(ll2)._node == NULL);
//    CH_ASSERT(hash_map_end(ll2).value == NULL);
//    CH_ASSERT(ll2->count == 0);
//    CH_ASSERT(ll2->_cmp == (cmp_void_f)cmp_i64);
//
//    CH_ASSERT(it4._node == NULL);
//    CH_ASSERT(it4.value == NULL);
//
//    //Check equality
//    CH_ASSERT(hash_map_eq(ll1,ll2));
//
//    //Clean up and bug out.
//    hash_map_delete(ll1);
//    hash_map_delete(ll2);
//    return result;
//}
//
//
//#define TEST_4_MAX 15
//
///* Insert all elements into an empty hash_map. Use both push back, push front and push_back_carray*/
//static ch_word test4_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    //Make new hash_maps
//    ch_hash_map* ll1 = ch_hash_map_new(sizeof(i64),cmp_i64);
//    ch_hash_map* ll2 = ch_hash_map_new(sizeof(i64),cmp_i64);
//    ch_hash_map* ll3 = ch_hash_map_new(sizeof(i64),cmp_i64);
//
//    for(ch_word i = 0; i < TEST_4_MAX; i++){
//        //Push a value onto it
//        ch_hash_map_it it1 = hash_map_push_back(ll1, &test_data[i]);
//
//        CH_ASSERT(!hash_map_eq(ll1,ll2));
//
//        ch_hash_map_it it4 = hash_map_push_front(ll2, &test_data[TEST_4_MAX -1 - i]);
//
//        //Test the the push returns a valid iterator
//        CH_ASSERT( it1._node );
//        CH_ASSERT( (*(i64*)NODE_DATAP(it1._node)) == test_data[i] );
//        CH_ASSERT( it1.value );
//        CH_ASSERT( *(i64*)it1.value == test_data[i] );
//
//        //Test that first() returns a valid iterator
//        ch_hash_map_it it2 = hash_map_first(ll1);
//        CH_ASSERT( it2._node );
//        CH_ASSERT( (*(i64*)NODE_DATAP(it2._node)) == test_data[0] );
//        CH_ASSERT( it2.value );
//        CH_ASSERT( *(i64*)it2.value == test_data[0] );
//
//        //Test that last() returns a valid iterator
//        ch_hash_map_it it3 = hash_map_last(ll1);
//        CH_ASSERT( it3._node );
//        CH_ASSERT( (*(i64*)NODE_DATAP(it1._node)) == test_data[i] );
//        CH_ASSERT( it3.value );
//        CH_ASSERT( *(i64*)it3.value == test_data[i] );
//
//        //Test that the returned iterator is valid
//        CH_ASSERT( it4._node );
//        CH_ASSERT( (*(i64*)NODE_DATAP(it4._node)) == test_data[TEST_4_MAX -1 -i] );
//        CH_ASSERT( it4.value );
//        CH_ASSERT( *(i64*)it4.value == test_data[TEST_4_MAX -1 -i] );
//
//        //Test that first() returns a valid iterator
//        ch_hash_map_it it5 = hash_map_first(ll2);
//        CH_ASSERT( it5._node );
//        CH_ASSERT( (*(i64*)NODE_DATAP(it5._node)) == test_data[TEST_4_MAX -1 - i] );
//        CH_ASSERT( it5.value );
//        CH_ASSERT( *(i64*)it5.value == test_data[TEST_4_MAX -1  -i] );
//
//        //Test that last() returns a valid iterator
//        ch_hash_map_it it7 = hash_map_last(ll2);
//        CH_ASSERT( it7._node );
//        CH_ASSERT( (*(i64*)NODE_DATAP(it7._node)) == test_data[TEST_4_MAX -1] );
//        CH_ASSERT( it7.value );
//        CH_ASSERT( *(i64*)it7.value == test_data[TEST_4_MAX -1] );
//    }
//
//    ch_hash_map_it it8 = hash_map_push_back_carray(ll3, test_data, TEST_4_MAX);
//    //Test that the returned iterator is valid
//    CH_ASSERT( it8._node );
//    CH_ASSERT( (*(i64*)NODE_DATAP(it8._node)) == test_data[TEST_4_MAX -1] );
//    CH_ASSERT( it8.value );
//    CH_ASSERT( *(i64*)it8.value == test_data[TEST_4_MAX -1] );
//
//    //Test that first() returns a valid iterator
//    ch_hash_map_it it9 = hash_map_first(ll2);
//    CH_ASSERT( it9._node );
//    CH_ASSERT( (*(i64*)NODE_DATAP(it9._node)) == test_data[0] );
//    CH_ASSERT( it9.value );
//    CH_ASSERT( *(i64*)it9.value == test_data[0] );
//
//    //Test that last() returns a valid iterator
//    ch_hash_map_it it10 = hash_map_last(ll2);
//    CH_ASSERT( it10._node );
//    CH_ASSERT( (*(i64*)NODE_DATAP(it10._node)) == test_data[TEST_4_MAX -1] );
//    CH_ASSERT( it10.value );
//    CH_ASSERT( *(i64*)it10.value == test_data[TEST_4_MAX -1] );
//
//    //Check that the hash_maps are now equal
//    CH_ASSERT(hash_map_eq(ll1,ll2));
//    CH_ASSERT(hash_map_eq(ll2,ll1));
//    CH_ASSERT(hash_map_eq(ll3,ll1));
//    CH_ASSERT(hash_map_eq(ll3,ll2));
//    CH_ASSERT(hash_map_eq(ll1,ll3));
//    CH_ASSERT(hash_map_eq(ll2,ll3));
//
//    //Clean up and bug out.
//    hash_map_delete(ll1);
//    hash_map_delete(ll2);
//    hash_map_delete(ll3);
//    return result;
//}
//
//
//#define TEST_5_MAX 15
//
///* Insert all elements into an empty hash_map. Use both push back, push front and push_back_carray
// * Then delete all elements from the front, back and middle.
// */
//static ch_word test5_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    //Make new hash_maps
//    ch_hash_map* ll1 = ch_hash_map_new(sizeof(i64),cmp_i64);
//    ch_hash_map* ll2 = ch_hash_map_new(sizeof(i64),cmp_i64);
//    ch_hash_map* ll3 = ch_hash_map_new(sizeof(i64),cmp_i64);
//
//    for(ch_word i = 0; i < TEST_5_MAX; i++){
//        hash_map_push_back(ll1, &test_data[i]);
//        hash_map_push_front(ll2, &test_data[TEST_5_MAX -1 - i]);
//    }
//    hash_map_push_back_carray(ll3, test_data, TEST_5_MAX);
//
//    //Check that the hash_maps are now equal
//    CH_ASSERT(hash_map_eq(ll1,ll2));
//    CH_ASSERT(hash_map_eq(ll2,ll1));
//    CH_ASSERT(hash_map_eq(ll3,ll1));
//    CH_ASSERT(hash_map_eq(ll3,ll2));
//    CH_ASSERT(hash_map_eq(ll1,ll3));
//    CH_ASSERT(hash_map_eq(ll2,ll3));
//
//    for(ch_word i = 0; i < TEST_5_MAX + 10; i++){
//        hash_map_pop_back(ll1);
//        hash_map_pop_front(ll2);
//        //dump_hash_map_i64(ll2);
//        //dump_hash_map_i64(ll2);
//    }
//
//    ch_hash_map_it it1 = hash_map_off(ll3,8);
//    while( (it1 = hash_map_remove(ll3,&it1) )._node ){
//        //dump_hash_map_i64(ll3);
//    }
//
//    CH_ASSERT( it1._node == NULL );
//    ch_hash_map* ll4 = ch_hash_map_new(sizeof(i64),cmp_i64);
//    hash_map_push_back_carray(ll4, test_data, 8);
//    CH_ASSERT(hash_map_eq(ll4,ll3));
//    CH_ASSERT(hash_map_eq(ll3,ll4));
//
//    ch_hash_map_it it2 = hash_map_first(ll3);
//    while( (it2 = hash_map_remove(ll3,&it2) )._node ){
//        //dump_hash_map_i64(ll3);
//    }
//
//    CH_ASSERT(hash_map_eq(ll1,ll2));
//    CH_ASSERT(hash_map_eq(ll2,ll1));
//    CH_ASSERT(hash_map_eq(ll3,ll1));
//    CH_ASSERT(hash_map_eq(ll3,ll2));
//    CH_ASSERT(hash_map_eq(ll1,ll3));
//    CH_ASSERT(hash_map_eq(ll2,ll3));
//
//
//    //Clean up and bug out.
//    hash_map_delete(ll1);
//    hash_map_delete(ll2);
//    hash_map_delete(ll3);
//    return result;
//}
//
//
////Try to find something in an empty hash_map
//static ch_word test6_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_hash_map* ll1 = ch_hash_map_new(sizeof(i64),cmp_i64);
//
//    ch_hash_map_it start = hash_map_first(ll1);
//    ch_hash_map_it end = hash_map_end(ll1);
//    CH_ASSERT(hash_map_find(ll1, &start, &end , &test_data[1])._node == NULL);
//    CH_ASSERT(hash_map_find(ll1, &start, &end , &test_data[1]).value == NULL);
//
//    hash_map_delete(ll1);
//
//    return result;
//}
//
//
//static ch_word test7_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_hash_map* ll1 = ch_hash_map_new(sizeof(i64),cmp_i64);
//    hash_map_push_back_carray(ll1, test_data, 3);
//
//    ch_hash_map_it start = hash_map_first(ll1);
//    ch_hash_map_it end = hash_map_end(ll1);
//    ch_hash_map_it it = hash_map_find(ll1, &start, &end , &test_data[1]);
//    CH_ASSERT(it._node);
//    CH_ASSERT(it.value);
//    CH_ASSERT(*(i64*)it.value == test_data[1]);
//
//    hash_map_delete(ll1);
//
//    return result;
//}
//
//
//static ch_word test8_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_hash_map* ll1 = ch_hash_map_new(sizeof(i64),cmp_i64);
//    hash_map_push_back_carray(ll1, test_data, 3);
//
//    ch_hash_map_it start = hash_map_first(ll1);
//    ch_hash_map_it end = hash_map_end(ll1);
//    ch_hash_map_it start1 = start; hash_map_forward(ll1,&start1,1);
//    ch_hash_map_it start2 = start; hash_map_forward(ll1,&start2,2);
//
//    i64 value = 42;
//    CH_ASSERT(hash_map_find(ll1,&start, &end, &value)._node == NULL);
//    CH_ASSERT(hash_map_find(ll1,&start, &end, &value).value == NULL);
//
//    CH_ASSERT(hash_map_find(ll1,&end, &end, &value)._node == NULL);
//    CH_ASSERT(hash_map_find(ll1,&end, &end, &value).value == NULL);
//
//    CH_ASSERT(hash_map_find(ll1,&start, &start1, &test_data[1])._node == NULL);
//    CH_ASSERT(hash_map_find(ll1,&start, &start1, &test_data[1]).value == NULL);
//
//    CH_ASSERT(hash_map_find(ll1,&start2, &start2, &test_data[2])._node == NULL);
//    CH_ASSERT(hash_map_find(ll1,&start2, &start2, &test_data[2]).value == NULL);
//
//    CH_ASSERT(hash_map_find(ll1,&start, &start2, &test_data[1])._node != NULL);
//    CH_ASSERT(hash_map_find(ll1,&start, &start2, &test_data[1]).value != NULL);
//    CH_ASSERT(*(i64*)hash_map_find(ll1,&start, &start2, &test_data[1]).value == test_data[1]);
//
//    hash_map_delete(ll1);
//
//    return result;
//}



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    i64 test_array[15] = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};
    //ch_word test_array_sorted[150] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9};

    ch_word test_result = 0;

    printf("CH Data Structures: Generic Linked List Test 01: ");  printf("%s", (test_result = test1_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 02: ");  printf("%s", (test_result = test2_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 03: ");  printf("%s", (test_result = test3_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 04: ");  printf("%s", (test_result = test4_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 05: ");  printf("%s", (test_result = test5_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 06: ");  printf("%s", (test_result = test6_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 07: ");  printf("%s", (test_result = test7_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 08: ");  printf("%s", (test_result = test8_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 09: ");  printf("%s", (test_result = test9_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 10: ");  printf("%s", (test_result = test10_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 11: ");  printf("%s", (test_result = test11_i64(test_array, test_array_sorted)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 12: ");  printf("%s", (test_result = test12_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 13: ");  printf("%s", (test_result = test13_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 14: ");  printf("%s", (test_result = test14_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Linked List Test 15: ");  printf("%s", (test_result = test16_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;

    return 0;
}



