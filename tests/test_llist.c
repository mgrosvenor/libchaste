// CamIO 2: demo_array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../data_structs/linked_list/linked_list.h"
#include "../utils/util.h"

#include <stdio.h>


static ch_word cmp_i64(void* lhs, void* rhs)
{
    if(*(i64*)lhs < *(i64*)rhs){
        return -1;
    }

    if(*(i64*)lhs > *(i64*)rhs){
        return 1;
    }

    return 0;
}


void dump_array_ch_word(ch_llist_t* ll)
{
    printf("There are %li items in the list\n", ll->count);
    for(ch_llist_it i = llist_first(ll); i.value ;  llist_next(ll, &i)){
        printf("%li,", (i64)i.value);
    }
    printf("\n");
}


static ch_word test1_i64(ch_word* test_data)
{
    ch_word result = 1;
    /* Test the initial conditions and equality of new (empty) array lists*/
    (void)test_data;
    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
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

    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
    CH_ASSERT(ll2 != ll1);
    CH_ASSERT(llist_eq(ll1,ll2));
    CH_ASSERT(llist_eq(ll2,ll1));
    llist_delete(ll1);
    llist_delete(ll2);
    return result;
}
//
//
///* Test the initial conditions and equality of new non-empty array lists*/
//static ch_word test2_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    (void)test_data;
//    ch_llist_t* ll1 = ch_llist_new(1,cmp_i64);
//    CH_ASSERT(ll1->_llist->first);
//    CH_ASSERT(ll1->first == ll1->_llist->first);
//    CH_ASSERT(ll1->last == ll1->first);
//    CH_ASSERT(ll1->end == ll1->last);
//    CH_ASSERT(ll1->_llist->count == 0);
//    CH_ASSERT(ll1->_llist->size == 1);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 0);
//    CH_ASSERT(ll1->size == 1);
//
//    ch_llist_t* ll2 = ch_llist_new(1,cmp_i64);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(ll1->eq(ll1,ll2));
//    CH_ASSERT(ll1->eq(ll2,ll1));
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//
//    return result;
//}
//
///* Insert an element into an array of size 0. Use both push back and push front*/
//static ch_word test3_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    CH_ASSERT(ll1->push_back(ll1, test_data[0]));
//    CH_ASSERT(ll1->_llist->first);
//    CH_ASSERT(ll1->first == ll1->_llist->first);
//    CH_ASSERT(ll1->last == ll1->first);
//    CH_ASSERT(ll1->end != ll1->last);
//    CH_ASSERT(*ll1->first == test_data[0]);
//    CH_ASSERT(*ll1->last == test_data[0]);
//    CH_ASSERT(ll1->_llist->count == 1);
//    CH_ASSERT(ll1->_llist->size == 1);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 1);
//    CH_ASSERT(ll1->size == 1);
//
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(!ll1->eq(ll1,ll2));
//    CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//
//    CH_ASSERT(ll1->eq(ll1,ll2));
//    CH_ASSERT(ll2->eq(ll2,ll1));
//    CH_ASSERT(ll1->first != ll2->first);
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//
//
///* Insrt an element into an array of size 1*/
//static ch_word test4_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(1,cmp_i64);
//    CH_ASSERT(ll1->push_back(ll1, test_data[0]));
//    CH_ASSERT(ll1->_llist->first);
//    CH_ASSERT(ll1->first == ll1->_llist->first);
//    CH_ASSERT(ll1->last == ll1->first);
//    CH_ASSERT(ll1->end != ll1->last);
//    CH_ASSERT(*ll1->first == test_data[0]);
//    CH_ASSERT(*ll1->last == test_data[0]);
//    CH_ASSERT(ll1->_llist->count == 1);
//    CH_ASSERT(ll1->_llist->size == 1);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 1);
//    CH_ASSERT(ll1->size == 1);
//
//    ch_llist_t* ll2 = ch_llist_new(1,cmp_i64);
//    CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(ll1->eq(ll1,ll2));
//    CH_ASSERT(ll2->eq(ll2,ll1));
//    CH_ASSERT(ll1->first != ll2->first);
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
///* Insert an element into an array of size 10*/
//static ch_word test5_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(10,cmp_i64);
//    CH_ASSERT(ll1->push_back(ll1, test_data[0]));
//    CH_ASSERT(ll1->_llist->first);
//    CH_ASSERT(ll1->first == ll1->_llist->first);
//    CH_ASSERT(ll1->last == ll1->first);
//    CH_ASSERT(ll1->end != ll1->last);
//    CH_ASSERT(*ll1->first == test_data[0]);
//    CH_ASSERT(*ll1->last == test_data[0]);
//    CH_ASSERT(ll1->_llist->count == 1);
//    CH_ASSERT(ll1->_llist->size == 10);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 1);
//    CH_ASSERT(ll1->size == 10);
//
//    ch_llist_t* ll2 = ch_llist_new(10,cmp_i64);
//    CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(ll1->eq(ll1,ll2));
//    CH_ASSERT(ll2->eq(ll2,ll1));
//    CH_ASSERT(ll1->first != ll2->first);
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//
///* Insert 2 elements into an array of size 0 and 1. Use both push back and push front*/
//static ch_word test6_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    CH_ASSERT(ll1->push_back(ll1, test_data[0]));
//    CH_ASSERT(ll1->push_back(ll1, test_data[1]));
//    CH_ASSERT(ll1->_llist->first);
//    CH_ASSERT(ll1->first == ll1->_llist->first);
//    CH_ASSERT(ll1->last != ll1->first);
//    CH_ASSERT(ll1->end != ll1->last);
//    CH_ASSERT(*ll1->first == test_data[0]);
//    CH_ASSERT(*ll1->last == test_data[1]);
//    CH_ASSERT(ll1->_llist->count == 2);
//    CH_ASSERT(ll1->_llist->size == 2);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 2);
//    CH_ASSERT(ll1->size == 2);
//
//    ch_llist_t* ll2 = ch_llist_new(1,cmp_i64);
//    CH_ASSERT(ll2->first == ll2->_llist->first);
//    CH_ASSERT(ll2->last == ll2->first);
//    CH_ASSERT(ll2->end == ll2->last);
//
//
//    CH_ASSERT(ll2->push_front(ll2, test_data[1]));
//    CH_ASSERT(ll2->first == ll2->_llist->first);
//    CH_ASSERT(ll2->last == ll2->first);
//    CH_ASSERT(ll2->end != ll2->last);
//    CH_ASSERT(*ll2->first == test_data[1]);
//
//    CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//    CH_ASSERT(ll2->first == ll2->_llist->first);
//    CH_ASSERT(ll2->last != ll2->first);
//    CH_ASSERT(ll2->end != ll2->last);
//
//    CH_ASSERT(*ll2->first == test_data[0]);
//    CH_ASSERT(*ll2->last == test_data[1]);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(ll1->eq(ll1,ll2));
//    CH_ASSERT(ll2->eq(ll2,ll1));
//    CH_ASSERT(ll1->first != ll2->first);
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//
///* Insert 2 elements into an array of size 0 and 1. Remove the head element. */
//static ch_word test7_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//    ch_word* rmv_1 = NULL;
//    ch_word* rmv_2 = NULL;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ch_llist_t* ll2 = ch_llist_new(2,cmp_i64);
//
//    /* Make sure that this works a few times over */
//    for(ch_word i = 0; i < 10; i++){
//
//        CH_ASSERT(ll1->push_back(ll1, test_data[0]));
//        CH_ASSERT(ll1->push_back(ll1, test_data[1]));
//        CH_ASSERT(ll1->first == ll1->_llist->first);
//
//        CH_ASSERT(ll2->push_front(ll2, test_data[1]));
//        CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//
//        rmv_1 = ll1->first;
//        rmv_2 = ll2->first;
//        CH_ASSERT(*ll1->remove(ll1,rmv_1) == test_data[1]);
//        CH_ASSERT(*ll2->remove(ll2,rmv_2) == test_data[1]);
//
//        CH_ASSERT(ll1->last == ll1->first);
//        CH_ASSERT(ll1->end != ll1->last);
//        CH_ASSERT(*ll1->first == test_data[1]);
//        CH_ASSERT(*ll1->last == test_data[1]);
//        CH_ASSERT(ll1->_llist->count == 1);
//        CH_ASSERT(ll1->_llist->size == 2);
//        CH_ASSERT(ll1->count == 1);
//        CH_ASSERT(ll1->size == 2);
//
//        CH_ASSERT(ll2->last == ll2->first);
//        CH_ASSERT(ll2->end != ll2->last);
//        CH_ASSERT(*ll2->first == test_data[1]);
//        CH_ASSERT(*ll2->last == test_data[1]);
//        CH_ASSERT(ll2->_llist->count == 1);
//        CH_ASSERT(ll2->_llist->size == 2);
//        CH_ASSERT(ll2->count == 1);
//        CH_ASSERT(ll2->size == 2);
//
//        CH_ASSERT(ll1->eq(ll1,ll2));
//        CH_ASSERT(ll2->eq(ll2,ll1));
//        CH_ASSERT(ll1->first != ll2->first);
//
//        CH_ASSERT(ll1->remove(ll1,ll1->first) == NULL);
//        CH_ASSERT(ll2->remove(ll2,ll2->last) == NULL);
//
//        CH_ASSERT(ll1->last == ll1->first);
//        CH_ASSERT(ll1->end == ll1->last);
//        CH_ASSERT(ll1->_llist->count == 0);
//        CH_ASSERT(ll1->_llist->size == 2);
//        CH_ASSERT(ll1->count == 0);
//        CH_ASSERT(ll1->size == 2);
//
//        CH_ASSERT(ll2->last == ll2->first);
//        CH_ASSERT(ll2->end == ll2->last);
//        CH_ASSERT(ll2->_llist->count == 0);
//        CH_ASSERT(ll2->_llist->size == 2);
//        CH_ASSERT(ll2->count == 0);
//        CH_ASSERT(ll2->size == 2);
//
//        CH_ASSERT(ll1->eq(ll1,ll2));
//        CH_ASSERT(ll2->eq(ll2,ll1));
//    }
//
//
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//
//static ch_word test8_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    (void)test_data;
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//
//    ll1->sort(ll1);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(ll1->_llist->first == NULL);
//    CH_ASSERT(ll1->first == NULL);
//    CH_ASSERT(ll1->last == NULL);
//    CH_ASSERT(ll1->end == NULL);
//    CH_ASSERT(ll1->_llist->count == 0);
//    CH_ASSERT(ll1->_llist->size == 0);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 0);
//    CH_ASSERT(ll1->size == 0);
//
//    CH_ASSERT(ll1->eq(ll1,ll2));
//    CH_ASSERT(ll1->eq(ll2,ll1));
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//static ch_word test9_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//
//    CH_ASSERT(ll1->push_back(ll1, test_data[0]));
//    CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//
//    ll1->sort(ll1);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(ll1->_llist->first);
//    CH_ASSERT(ll1->first == ll1->_llist->first);
//    CH_ASSERT(ll1->last == ll1->last);
//    CH_ASSERT(ll1->end == ll1->last + 1);
//    CH_ASSERT(ll1->_llist->count == 1);
//    CH_ASSERT(ll1->_llist->size == 1);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 1);
//    CH_ASSERT(ll1->size == 1);
//
//    CH_ASSERT(ll1->eq(ll1,ll2));
//    CH_ASSERT(ll1->eq(ll2,ll1));
//
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//static ch_word test10_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//
//    CH_ASSERT(ll1->push_back(ll1, test_data[0]));
//    CH_ASSERT(ll1->push_back(ll1, test_data[1]));
//    CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//    CH_ASSERT(ll2->push_front(ll2, test_data[1]));
//
//    ll1->sort(ll1);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(ll1->_llist->first);
//    CH_ASSERT(ll1->first == ll1->_llist->first);
//    CH_ASSERT(ll1->last == ll1->last);
//    CH_ASSERT(ll1->end == ll1->last + 1);
//    CH_ASSERT(ll1->_llist->count == 2);
//    CH_ASSERT(ll1->_llist->size == 2);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 2);
//    CH_ASSERT(ll1->size == 2);
//
//    CH_ASSERT(ll1->eq(ll1,ll2));
//    CH_ASSERT(ll1->eq(ll2,ll1));
//
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
////Push two carrays on and make sure the results are the same.
//static ch_word test11_i64(ch_word* test_data, ch_word* test_result)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//
//    for(ch_word i = 0; i < 10; i++){
//        ll1->push_back_carray(ll1,test_data, 15);
//
//        CH_ASSERT(ll1->first == ll1->_llist->first);
//        CH_ASSERT(ll1->last == ll1->last);
//        CH_ASSERT(ll1->end == ll1->last + 1);
//
//        ll1->sort(ll1);
//
//        CH_ASSERT(ll1->first == ll1->_llist->first);
//        CH_ASSERT(ll1->last == ll1->last);
//        CH_ASSERT(ll1->end == ll1->last + 1);
//
//    }
//
//    ll2->push_back_carray(ll2,test_result, 150);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(ll1->_llist->first);
//    CH_ASSERT(ll1->first == ll1->_llist->first);
//    CH_ASSERT(ll1->last == ll1->last);
//    CH_ASSERT(ll1->end == ll1->last + 1);
//    CH_ASSERT(ll1->_llist->count == 150);
//    CH_ASSERT(ll1->_llist->size == 256);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 150);
//    CH_ASSERT(ll1->size == 256);
//
//    CH_ASSERT(ll1->eq(ll1,ll2));
//    CH_ASSERT(ll1->eq(ll2,ll1));
//
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//
//static ch_word test12_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//
//    CH_ASSERT(ll1->find(ll1,ll1->first, ll1->end, test_data[1]) == NULL);
//
//
//    ll1->delete(ll1);
//
//    return result;
//}
//
//
//static ch_word test13_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ll1->push_back_carray(ll1, test_data, 3);
//
//    CH_ASSERT(*ll1->find(ll1,ll1->first, ll1->end, test_data[1]) == test_data[1]);
//
//    ll1->delete(ll1);
//
//    return result;
//}
//
//static ch_word test14_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ll1->push_back_carray(ll1, test_data, 3);
//
//    CH_ASSERT(ll1->find(ll1,ll1->first, ll1->end, 42) == NULL);
//    CH_ASSERT(ll1->find(ll1,ll1->end, ll1->end, 42) == NULL);
//    CH_ASSERT(ll1->find(ll1,ll1->first, ll1->first + 1, test_data[1]) == NULL);
//    CH_ASSERT(ll1->find(ll1,ll1->first + 2, ll1->first + 2, test_data[2]) == NULL);
//    CH_ASSERT(*ll1->find(ll1,ll1->first, ll1->first + 2, test_data[1]) == test_data[1]);
//
//    ll1->delete(ll1);
//
//    return result;
//}
//
//
///* Found a bug in the equaility operator. This should stimulate it*/
//static ch_word test16_i64(ch_word* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    CH_ASSERT(ll1->push_back(ll1, test_data[0]));
//    CH_ASSERT(ll1->_llist->first);
//    CH_ASSERT(ll1->first == ll1->_llist->first);
//    CH_ASSERT(ll1->last == ll1->first);
//    CH_ASSERT(ll1->end != ll1->last);
//    CH_ASSERT(*ll1->first == test_data[0]);
//    CH_ASSERT(*ll1->last == test_data[0]);
//    CH_ASSERT(ll1->_llist->count == 1);
//    CH_ASSERT(ll1->_llist->size == 1);
//    CH_ASSERT(ll1->_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(ll1->count == 1);
//    CH_ASSERT(ll1->size == 1);
//
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//    CH_ASSERT(!ll1->eq(ll1,ll2));
//    CH_ASSERT(ll2->push_front(ll2, test_data[1]));
//
//    CH_ASSERT(ll2 != ll1);
//
//    CH_ASSERT(!ll1->eq(ll1,ll2));
//    CH_ASSERT(!ll2->eq(ll2,ll1));
//    CH_ASSERT(ll1->first != ll2->first);
//    ll1->delete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    ch_word test_array[15] = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};
    //ch_word test_array_sorted[150] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9};

    ch_word test_result = 0;

    printf("CH Data Structures: Vector Test 01: ");  printf("%s", (test_result = test1_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 02: ");  printf("%s", (test_result = test2_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 03: ");  printf("%s", (test_result = test3_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 04: ");  printf("%s", (test_result = test4_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 05: ");  printf("%s", (test_result = test5_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 06: ");  printf("%s", (test_result = test6_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 07: ");  printf("%s", (test_result = test7_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 08: ");  printf("%s", (test_result = test8_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 09: ");  printf("%s", (test_result = test9_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 10: ");  printf("%s", (test_result = test10_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 11: ");  printf("%s", (test_result = test11_i64(test_array, test_array_sorted)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 12: ");  printf("%s", (test_result = test12_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 13: ");  printf("%s", (test_result = test13_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 14: ");  printf("%s", (test_result = test14_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Vector Test 15: ");  printf("%s", (test_result = test16_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;

    return 0;
}


