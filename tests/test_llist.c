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
    ch_llist_it it3 = llist_first(ll1);
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
    ch_llist_it it7 = llist_first(ll2);
    CH_ASSERT( it7._node );
    CH_ASSERT( (*(i64*)NODE_DATAP(it1._node)) == test_data[0] );
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




///* Insert an element into an array of size 10*/
//static ch_word test5_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(10,cmp_i64);
//    CH_ASSERT(llistpush_back(ll1, test_data[0]));
//    CH_ASSERT(llist_llist->first);
//    CH_ASSERT(llistfirst == llist_llist->first);
//    CH_ASSERT(llistlast == llistfirst);
//    CH_ASSERT(llistend != llistlast);
//    CH_ASSERT(*llistfirst == test_data[0]);
//    CH_ASSERT(*llistlast == test_data[0]);
//    CH_ASSERT(llist_llist->count == 1);
//    CH_ASSERT(llist_llist->size == 10);
//    CH_ASSERT(llist_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(llistcount == 1);
//    CH_ASSERT(llistsize == 10);
//
//    ch_llist_t* ll2 = ch_llist_new(10,cmp_i64);
//    CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(llisteq(ll1,ll2));
//    CH_ASSERT(ll2->eq(ll2,ll1));
//    CH_ASSERT(llistfirst != ll2->first);
//    llistdelete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//
///* Insert 2 elements into an array of size 0 and 1. Use both push back and push front*/
//static ch_word test6_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    CH_ASSERT(llistpush_back(ll1, test_data[0]));
//    CH_ASSERT(llistpush_back(ll1, test_data[1]));
//    CH_ASSERT(llist_llist->first);
//    CH_ASSERT(llistfirst == llist_llist->first);
//    CH_ASSERT(llistlast != llistfirst);
//    CH_ASSERT(llistend != llistlast);
//    CH_ASSERT(*llistfirst == test_data[0]);
//    CH_ASSERT(*llistlast == test_data[1]);
//    CH_ASSERT(llist_llist->count == 2);
//    CH_ASSERT(llist_llist->size == 2);
//    CH_ASSERT(llist_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(llistcount == 2);
//    CH_ASSERT(llistsize == 2);
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
//    CH_ASSERT(llisteq(ll1,ll2));
//    CH_ASSERT(ll2->eq(ll2,ll1));
//    CH_ASSERT(llistfirst != ll2->first);
//    llistdelete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//
///* Insert 2 elements into an array of size 0 and 1. Remove the head element. */
//static ch_word test7_i64(i64* test_data)
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
//        CH_ASSERT(llistpush_back(ll1, test_data[0]));
//        CH_ASSERT(llistpush_back(ll1, test_data[1]));
//        CH_ASSERT(llistfirst == llist_llist->first);
//
//        CH_ASSERT(ll2->push_front(ll2, test_data[1]));
//        CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//
//        rmv_1 = llistfirst;
//        rmv_2 = ll2->first;
//        CH_ASSERT(*llistremove(ll1,rmv_1) == test_data[1]);
//        CH_ASSERT(*ll2->remove(ll2,rmv_2) == test_data[1]);
//
//        CH_ASSERT(llistlast == llistfirst);
//        CH_ASSERT(llistend != llistlast);
//        CH_ASSERT(*llistfirst == test_data[1]);
//        CH_ASSERT(*llistlast == test_data[1]);
//        CH_ASSERT(llist_llist->count == 1);
//        CH_ASSERT(llist_llist->size == 2);
//        CH_ASSERT(llistcount == 1);
//        CH_ASSERT(llistsize == 2);
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
//        CH_ASSERT(llisteq(ll1,ll2));
//        CH_ASSERT(ll2->eq(ll2,ll1));
//        CH_ASSERT(llistfirst != ll2->first);
//
//        CH_ASSERT(llistremove(ll1,llistfirst) == NULL);
//        CH_ASSERT(ll2->remove(ll2,ll2->last) == NULL);
//
//        CH_ASSERT(llistlast == llistfirst);
//        CH_ASSERT(llistend == llistlast);
//        CH_ASSERT(llist_llist->count == 0);
//        CH_ASSERT(llist_llist->size == 2);
//        CH_ASSERT(llistcount == 0);
//        CH_ASSERT(llistsize == 2);
//
//        CH_ASSERT(ll2->last == ll2->first);
//        CH_ASSERT(ll2->end == ll2->last);
//        CH_ASSERT(ll2->_llist->count == 0);
//        CH_ASSERT(ll2->_llist->size == 2);
//        CH_ASSERT(ll2->count == 0);
//        CH_ASSERT(ll2->size == 2);
//
//        CH_ASSERT(llisteq(ll1,ll2));
//        CH_ASSERT(ll2->eq(ll2,ll1));
//    }
//
//
//    llistdelete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//
//static ch_word test8_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    (void)test_data;
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//
//    llistsort(ll1);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(llist_llist->first == NULL);
//    CH_ASSERT(llistfirst == NULL);
//    CH_ASSERT(llistlast == NULL);
//    CH_ASSERT(llistend == NULL);
//    CH_ASSERT(llist_llist->count == 0);
//    CH_ASSERT(llist_llist->size == 0);
//    CH_ASSERT(llist_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(llistcount == 0);
//    CH_ASSERT(llistsize == 0);
//
//    CH_ASSERT(llisteq(ll1,ll2));
//    CH_ASSERT(llisteq(ll2,ll1));
//    llistdelete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//static ch_word test9_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//
//    CH_ASSERT(llistpush_back(ll1, test_data[0]));
//    CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//
//    llistsort(ll1);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(llist_llist->first);
//    CH_ASSERT(llistfirst == llist_llist->first);
//    CH_ASSERT(llistlast == llistlast);
//    CH_ASSERT(llistend == llistlast + 1);
//    CH_ASSERT(llist_llist->count == 1);
//    CH_ASSERT(llist_llist->size == 1);
//    CH_ASSERT(llist_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(llistcount == 1);
//    CH_ASSERT(llistsize == 1);
//
//    CH_ASSERT(llisteq(ll1,ll2));
//    CH_ASSERT(llisteq(ll2,ll1));
//
//    llistdelete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//static ch_word test10_i64(i64* test_data)
//{
//    ch_word result = 1;
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//
//    CH_ASSERT(llistpush_back(ll1, test_data[0]));
//    CH_ASSERT(llistpush_back(ll1, test_data[1]));
//    CH_ASSERT(ll2->push_front(ll2, test_data[0]));
//    CH_ASSERT(ll2->push_front(ll2, test_data[1]));
//
//    llistsort(ll1);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(llist_llist->first);
//    CH_ASSERT(llistfirst == llist_llist->first);
//    CH_ASSERT(llistlast == llistlast);
//    CH_ASSERT(llistend == llistlast + 1);
//    CH_ASSERT(llist_llist->count == 2);
//    CH_ASSERT(llist_llist->size == 2);
//    CH_ASSERT(llist_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(llistcount == 2);
//    CH_ASSERT(llistsize == 2);
//
//    CH_ASSERT(llisteq(ll1,ll2));
//    CH_ASSERT(llisteq(ll2,ll1));
//
//    llistdelete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
////Push two carrays on and make sure the results are the same.
//static ch_word test11_i64(i64* test_data, ch_word* test_result)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//
//    for(ch_word i = 0; i < 10; i++){
//        llistpush_back_carray(ll1,test_data, 15);
//
//        CH_ASSERT(llistfirst == llist_llist->first);
//        CH_ASSERT(llistlast == llistlast);
//        CH_ASSERT(llistend == llistlast + 1);
//
//        llistsort(ll1);
//
//        CH_ASSERT(llistfirst == llist_llist->first);
//        CH_ASSERT(llistlast == llistlast);
//        CH_ASSERT(llistend == llistlast + 1);
//
//    }
//
//    ll2->push_back_carray(ll2,test_result, 150);
//
//    CH_ASSERT(ll2 != ll1);
//    CH_ASSERT(llist_llist->first);
//    CH_ASSERT(llistfirst == llist_llist->first);
//    CH_ASSERT(llistlast == llistlast);
//    CH_ASSERT(llistend == llistlast + 1);
//    CH_ASSERT(llist_llist->count == 150);
//    CH_ASSERT(llist_llist->size == 256);
//    CH_ASSERT(llist_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(llistcount == 150);
//    CH_ASSERT(llistsize == 256);
//
//    CH_ASSERT(llisteq(ll1,ll2));
//    CH_ASSERT(llisteq(ll2,ll1));
//
//    llistdelete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//
//
//static ch_word test12_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//
//    CH_ASSERT(llistfind(ll1,llistfirst, llistend, test_data[1]) == NULL);
//
//
//    llistdelete(ll1);
//
//    return result;
//}
//
//
//static ch_word test13_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    llistpush_back_carray(ll1, test_data, 3);
//
//    CH_ASSERT(*llistfind(ll1,llistfirst, llistend, test_data[1]) == test_data[1]);
//
//    llistdelete(ll1);
//
//    return result;
//}
//
//static ch_word test14_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    llistpush_back_carray(ll1, test_data, 3);
//
//    CH_ASSERT(llistfind(ll1,llistfirst, llistend, 42) == NULL);
//    CH_ASSERT(llistfind(ll1,llistend, llistend, 42) == NULL);
//    CH_ASSERT(llistfind(ll1,llistfirst, llistfirst + 1, test_data[1]) == NULL);
//    CH_ASSERT(llistfind(ll1,llistfirst + 2, llistfirst + 2, test_data[2]) == NULL);
//    CH_ASSERT(*llistfind(ll1,llistfirst, llistfirst + 2, test_data[1]) == test_data[1]);
//
//    llistdelete(ll1);
//
//    return result;
//}
//
//
///* Found a bug in the equaility operator. This should stimulate it*/
//static ch_word test16_i64(i64* test_data)
//{
//    ch_word result = 1;
//
//    ch_llist_t* ll1 = ch_llist_new(0,cmp_i64);
//    CH_ASSERT(llistpush_back(ll1, test_data[0]));
//    CH_ASSERT(llist_llist->first);
//    CH_ASSERT(llistfirst == llist_llist->first);
//    CH_ASSERT(llistlast == llistfirst);
//    CH_ASSERT(llistend != llistlast);
//    CH_ASSERT(*llistfirst == test_data[0]);
//    CH_ASSERT(*llistlast == test_data[0]);
//    CH_ASSERT(llist_llist->count == 1);
//    CH_ASSERT(llist_llist->size == 1);
//    CH_ASSERT(llist_llist->_cmp == (cmp_void_f)cmp_i64);
//    CH_ASSERT(llistcount == 1);
//    CH_ASSERT(llistsize == 1);
//
//    ch_llist_t* ll2 = ch_llist_new(0,cmp_i64);
//    CH_ASSERT(!llisteq(ll1,ll2));
//    CH_ASSERT(ll2->push_front(ll2, test_data[1]));
//
//    CH_ASSERT(ll2 != ll1);
//
//    CH_ASSERT(!llisteq(ll1,ll2));
//    CH_ASSERT(!ll2->eq(ll2,ll1));
//    CH_ASSERT(llistfirst != ll2->first);
//    llistdelete(ll1);
//    ll2->delete(ll2);
//    return result;
//}
//


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    i64 test_array[15] = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};
    //ch_word test_array_sorted[150] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9};

    ch_word test_result = 0;

    printf("CH Data Structures: Linked List Test 01: ");  printf("%s", (test_result = test1_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Linked List Test 02: ");  printf("%s", (test_result = test2_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Linked List Test 03: ");  printf("%s", (test_result = test3_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 04: ");  printf("%s", (test_result = test4_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 05: ");  printf("%s", (test_result = test5_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 06: ");  printf("%s", (test_result = test6_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 07: ");  printf("%s", (test_result = test7_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 08: ");  printf("%s", (test_result = test8_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 09: ");  printf("%s", (test_result = test9_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 10: ");  printf("%s", (test_result = test10_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 11: ");  printf("%s", (test_result = test11_i64(test_array, test_array_sorted)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 12: ");  printf("%s", (test_result = test12_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 13: ");  printf("%s", (test_result = test13_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 14: ");  printf("%s", (test_result = test14_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Linked List Test 15: ");  printf("%s", (test_result = test16_i64(test_array)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;

    return 0;
}



