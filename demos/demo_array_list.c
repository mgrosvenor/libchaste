// CamIO 2: demo_array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../data_structs/array_list_std.h"
#include <stdio.h>

#define TYPE int

#define ch_array_list_t(TYPE) ch_array_list_##TYPE##_t


#define CH_ASSERT(p) do { if(!(p)) { fprintf(stderr, "Error in %s: failed assertion \""#p"\"\n", __FUNCTION__); result = 0; } } while(0)


static ch_word cmp_i64(i64 lhs, i64 rhs)
{
    if(lhs < rhs){
        return -1;
    }

    if(lhs > rhs){
        return 1;
    }

    return 0;
}


void dump_array_i64(ch_array_list_i64_t* al)
{
    printf("There are %li items in the list\n", al->count);
    for(i64* i = al->first; i < al->end; i = al->next(al, i)){
        printf("%li,", *i);
    }
    printf("\n");
}


static ch_word test1_i64(i64* test_data)
{
    ch_word result = 1;
    /* Test the initial conditions and equality of new (empty) array lists*/
    (void)test_data;
    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    CH_ASSERT(al1->_array_backing == NULL);
    CH_ASSERT(al1->first == NULL);
    CH_ASSERT(al1->last == NULL);
    CH_ASSERT(al1->end == NULL);
    CH_ASSERT(al1->_array_backing_count == 0);
    CH_ASSERT(al1->_array_backing_size == 0);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 0);
    CH_ASSERT(al1->size == 0);

    ch_array_list_i64_t* al2 = ch_array_list_i64_new(0,cmp_i64);
    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);
    return result;
}


/* Test the initial conditions and equality of new non-empty array lists*/
static ch_word test2_i64(i64* test_data)
{
    ch_word result = 1;

    (void)test_data;
    ch_array_list_i64_t* al1 = ch_array_list_i64_new(1,cmp_i64);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end == al1->last);
    CH_ASSERT(al1->_array_backing_count == 0);
    CH_ASSERT(al1->_array_backing_size == 1);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 0);
    CH_ASSERT(al1->size == 1);

    ch_array_list_i64_t* al2 = ch_array_list_i64_new(1,cmp_i64);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);

    return result;
}

/* Insrt an element into an array of size 0. Use both push back and push front*/
static ch_word test3_i64(i64* test_data)
{
    ch_word result = 1;

    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end != al1->last);
    CH_ASSERT(*al1->first == test_data[0]);
    CH_ASSERT(*al1->last == test_data[0]);
    CH_ASSERT(al1->_array_backing_count == 1);
    CH_ASSERT(al1->_array_backing_size == 1);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 1);
    CH_ASSERT(al1->size == 1);

    ch_array_list_i64_t* al2 = ch_array_list_i64_new(0,cmp_i64);
    CH_ASSERT(al2->push_front(al2, test_data[0]));

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al2->eq(al2,al1));
    CH_ASSERT(al1->first != al2->first);
    al1->delete(al1);
    al2->delete(al2);
    return result;
}



/* Insrt an element into an array of size 1*/
static ch_word test4_i64(i64* test_data)
{
    ch_word result = 1;

    ch_array_list_i64_t* al1 = ch_array_list_i64_new(1,cmp_i64);
    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end != al1->last);
    CH_ASSERT(*al1->first == test_data[0]);
    CH_ASSERT(*al1->last == test_data[0]);
    CH_ASSERT(al1->_array_backing_count == 1);
    CH_ASSERT(al1->_array_backing_size == 1);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 1);
    CH_ASSERT(al1->size == 1);

    ch_array_list_i64_t* al2 = ch_array_list_i64_new(1,cmp_i64);
    CH_ASSERT(al2->push_front(al2, test_data[0]));

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al2->eq(al2,al1));
    CH_ASSERT(al1->first != al2->first);
    al1->delete(al1);
    al2->delete(al2);
    return result;
}

/* Insert an element into an array of size 10*/
static ch_word test5_i64(i64* test_data)
{
    ch_word result = 1;

    ch_array_list_i64_t* al1 = ch_array_list_i64_new(10,cmp_i64);
    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end != al1->last);
    CH_ASSERT(*al1->first == test_data[0]);
    CH_ASSERT(*al1->last == test_data[0]);
    CH_ASSERT(al1->_array_backing_count == 1);
    CH_ASSERT(al1->_array_backing_size == 10);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 1);
    CH_ASSERT(al1->size == 10);

    ch_array_list_i64_t* al2 = ch_array_list_i64_new(10,cmp_i64);
    CH_ASSERT(al2->push_front(al2, test_data[0]));

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al2->eq(al2,al1));
    CH_ASSERT(al1->first != al2->first);
    al1->delete(al1);
    al2->delete(al2);
    return result;
}


/* Insert 2 elements into an array of size 0 and 1. Use both push back and push front*/
static ch_word test6_i64(i64* test_data)
{
    ch_word result = 1;

    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->push_back(al1, test_data[1]));
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last != al1->first);
    CH_ASSERT(al1->end != al1->last);
    CH_ASSERT(*al1->first == test_data[0]);
    CH_ASSERT(*al1->last == test_data[1]);
    CH_ASSERT(al1->_array_backing_count == 2);
    CH_ASSERT(al1->_array_backing_size == 2);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 2);
    CH_ASSERT(al1->size == 2);

    ch_array_list_i64_t* al2 = ch_array_list_i64_new(1,cmp_i64);
    CH_ASSERT(al2->push_front(al2, test_data[1]));
    CH_ASSERT(al2->push_front(al2, test_data[0]));

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al2->eq(al2,al1));
    CH_ASSERT(al1->first != al2->first);
    al1->delete(al1);
    al2->delete(al2);
    return result;
}


/* Insert 2 elements into an array of size 0 and 1. Remove the head element. */
static ch_word test7_i64(i64* test_data)
{
    ch_word result = 1;
    i64* rmv_1 = NULL;
    i64* rmv_2 = NULL;

    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    ch_array_list_i64_t* al2 = ch_array_list_i64_new(2,cmp_i64);

    /* Make sure that this works a few times over */
    for(ch_word i = 0; i < 10; i++){

        CH_ASSERT(rmv_1 = al1->push_back(al1, test_data[0]));
        CH_ASSERT(al1->push_back(al1, test_data[1]));
        CH_ASSERT(al1->first == al1->_array_backing);


        CH_ASSERT(al2->push_front(al2, test_data[1]));
        CH_ASSERT(rmv_2 = al2->push_front(al2, test_data[0]));

        CH_ASSERT(*al1->remove(al1,rmv_1) == test_data[1]);
        CH_ASSERT(*al2->remove(al2,rmv_2) == test_data[1]);

        CH_ASSERT(al1->last == al1->first);
        CH_ASSERT(al1->end != al1->last);
        CH_ASSERT(*al1->first == test_data[1]);
        CH_ASSERT(*al1->last == test_data[1]);
        CH_ASSERT(al1->_array_backing_count == 1);
        CH_ASSERT(al1->_array_backing_size == 2);
        CH_ASSERT(al1->count == 1);
        CH_ASSERT(al1->size == 2);

        CH_ASSERT(al2->last == al2->first);
        CH_ASSERT(al2->end != al2->last);
        CH_ASSERT(*al2->first == test_data[1]);
        CH_ASSERT(*al2->last == test_data[1]);
        CH_ASSERT(al2->_array_backing_count == 1);
        CH_ASSERT(al2->_array_backing_size == 2);
        CH_ASSERT(al2->count == 1);
        CH_ASSERT(al2->size == 2);

        CH_ASSERT(al1->eq(al1,al2));
        CH_ASSERT(al2->eq(al2,al1));
        CH_ASSERT(al1->first != al2->first);

        CH_ASSERT(al1->remove(al1,al1->first) == NULL);
        CH_ASSERT(al2->remove(al2,al2->last) == NULL);

        CH_ASSERT(al1->last == al1->first);
        CH_ASSERT(al1->end == al1->last);
        CH_ASSERT(al1->_array_backing_count == 0);
        CH_ASSERT(al1->_array_backing_size == 2);
        CH_ASSERT(al1->count == 0);
        CH_ASSERT(al1->size == 2);

        CH_ASSERT(al2->last == al2->first);
        CH_ASSERT(al2->end == al2->last);
        CH_ASSERT(al2->_array_backing_count == 0);
        CH_ASSERT(al2->_array_backing_size == 2);
        CH_ASSERT(al2->count == 0);
        CH_ASSERT(al2->size == 2);

        CH_ASSERT(al1->eq(al1,al2));
        CH_ASSERT(al2->eq(al2,al1));
    }


    al1->delete(al1);
    al2->delete(al2);
    return result;
}


static ch_word test8_i64(i64* test_data)
{
    ch_word result = 1;

    (void)test_data;
    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    ch_array_list_i64_t* al2 = ch_array_list_i64_new(0,cmp_i64);

    al1->sort(al1);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing == NULL);
    CH_ASSERT(al1->first == NULL);
    CH_ASSERT(al1->last == NULL);
    CH_ASSERT(al1->end == NULL);
    CH_ASSERT(al1->_array_backing_count == 0);
    CH_ASSERT(al1->_array_backing_size == 0);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 0);
    CH_ASSERT(al1->size == 0);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);
    return result;
}

static ch_word test9_i64(i64* test_data)
{
    ch_word result = 1;

    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    ch_array_list_i64_t* al2 = ch_array_list_i64_new(0,cmp_i64);

    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al2->push_front(al2, test_data[0]));

    al1->sort(al1);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->last);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_count == 1);
    CH_ASSERT(al1->_array_backing_size == 1);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 1);
    CH_ASSERT(al1->size == 1);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));

    al1->delete(al1);
    al2->delete(al2);
    return result;
}

static ch_word test10_i64(i64* test_data)
{
    ch_word result = 1;
    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    ch_array_list_i64_t* al2 = ch_array_list_i64_new(0,cmp_i64);

    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->push_back(al1, test_data[1]));
    CH_ASSERT(al2->push_front(al2, test_data[0]));
    CH_ASSERT(al2->push_front(al2, test_data[1]));

    al1->sort(al1);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->last);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_count == 2);
    CH_ASSERT(al1->_array_backing_size == 2);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 2);
    CH_ASSERT(al1->size == 2);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));

    al1->delete(al1);
    al2->delete(al2);
    return result;
}

static ch_word test11_i64(i64* test_data, i64* test_result)
{
    ch_word result = 1;
    /* Test the initial conditions and equality of new (empty) array lists*/
    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    ch_array_list_i64_t* al2 = ch_array_list_i64_new(0,cmp_i64);

    for(ch_word i = 0; i < 10; i++){
        al1->push_back_carray(al1,test_data, 15);
        al1->sort(al1);
    }

    al2->push_back_carray(al2,test_result, 150);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->last);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_count == 150);
    CH_ASSERT(al1->_array_backing_size == 256);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 150);
    CH_ASSERT(al1->size == 256);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));

    al1->delete(al1);
    al2->delete(al2);
    return result;
}


static ch_word test12_i64(i64* test_data)
{
    ch_word result = 1;

    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);

    CH_ASSERT(al1->find(al1,al1->first, al1->end, test_data[1]) == NULL);


    al1->delete(al1);

    return result;
}


static ch_word test13_i64(i64* test_data)
{
    ch_word result = 1;

    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    al1->push_back_carray(al1, test_data, 3);

    CH_ASSERT(*al1->find(al1,al1->first, al1->end, test_data[1]) == test_data[1]);

    al1->delete(al1);

    return result;
}

static ch_word test14_i64(i64* test_data)
{
    ch_word result = 1;

    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    al1->push_back_carray(al1, test_data, 3);

    CH_ASSERT(al1->find(al1,al1->first, al1->end, 42) == NULL);
    CH_ASSERT(al1->find(al1,al1->end, al1->end, 42) == NULL);
    CH_ASSERT(al1->find(al1,al1->first, al1->first + 1, test_data[1]) == NULL);
    CH_ASSERT(al1->find(al1,al1->first + 2, al1->first + 2, test_data[2]) == NULL);
    CH_ASSERT(*al1->find(al1,al1->first, al1->first + 2, test_data[1]) == test_data[1]);

    al1->delete(al1);

    return result;
}


static ch_word test15_i64(i64* test_data, i64* test_result)
{
    ch_word result = 1;
    /* Test the initial conditions and equality of new (empty) array lists*/
    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    ch_array_list_i64_t* al2 = ch_array_list_i64_new(0,cmp_i64);

    for(ch_word i = 0; i < 10; i++){
        al1->push_back_carray(al1,test_data, 15);
        al1->sort_reverse(al1);
    }

    al2->push_back_carray(al2,test_result, 150);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->last);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_count == 150);
    CH_ASSERT(al1->_array_backing_size == 256);
    CH_ASSERT(al1->_cmp == cmp_i64);
    CH_ASSERT(al1->count == 150);
    CH_ASSERT(al1->size == 256);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));

    al1->delete(al1);
    al2->delete(al2);
    return result;
}



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    i64 test_array[15] = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};
    i64 test_array_sorted[150] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9};
    i64 test_array_rsorted[150] = {9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,8,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0};
    printf("CH Data Structures: Array List Test 01: ");  printf("%s", test1_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 02: ");  printf("%s", test2_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 03: ");  printf("%s", test3_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 04: ");  printf("%s", test4_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 05: ");  printf("%s", test5_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 06: ");  printf("%s", test6_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 07: ");  printf("%s", test7_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 08: ");  printf("%s", test8_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 09: ");  printf("%s", test9_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 10: ");  printf("%s", test10_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 11: ");  printf("%s", test11_i64(test_array, test_array_sorted) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 12: ");  printf("%s", test12_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 13: ");  printf("%s", test13_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 14: ");  printf("%s", test14_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 15: ");  printf("%s", test15_i64(test_array, test_array_rsorted) ? "PASS\n" : "FAIL\n");

    return 0;
}


