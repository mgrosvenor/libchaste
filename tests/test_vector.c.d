// CamIO 2: demo_array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../data_structs/vector/vector_std.h"
#include "../utils/util.h"

#include <stdio.h>


#define ch_vector_t ch_vector_ch_word_t

static ch_word cmp_ch_word(ch_word lhs, ch_word rhs)
{
    if(lhs < rhs){
        return -1;
    }

    if(lhs > rhs){
        return 1;
    }

    return 0;
}


void dump_array_ch_word(ch_vector_t* al)
{
    printf("There are %li items in the list\n", al->count);
    for(ch_word* i = al->first; i < al->end; i = al->next(al, i)){
        printf("%li,", *i);
    }
    printf("\n");
}


static ch_word test1_ch_word(ch_word* test_data)
{
    ch_word result = 1;
    /* Test the initial conditions and equality of new (empty) array lists*/
    (void)test_data;
    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    CH_ASSERT(al1->_array_backing->first == NULL);
    CH_ASSERT(al1->first == NULL);
    CH_ASSERT(al1->last == NULL);
    CH_ASSERT(al1->end == NULL);
    CH_ASSERT(al1->_array_backing_count == 0);
    CH_ASSERT(al1->_array_backing->size == 0);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 0);
    CH_ASSERT(al1->size == 0);

    ch_vector_t* al2 = ch_vector_ch_word_new(0,cmp_ch_word);
    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);
    return result;
}


/* Test the initial conditions and equality of new non-empty array lists*/
static ch_word test2_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    (void)test_data;
    ch_vector_t* al1 = ch_vector_ch_word_new(1,cmp_ch_word);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end == al1->last);
    CH_ASSERT(al1->_array_backing_count == 0);
    CH_ASSERT(al1->_array_backing->size == 1);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 0);
    CH_ASSERT(al1->size == 1);

    ch_vector_t* al2 = ch_vector_ch_word_new(1,cmp_ch_word);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);

    return result;
}

/* Insert an element into an array of size 0. Use both push back and push front*/
static ch_word test3_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end != al1->last);
    CH_ASSERT(*al1->first == test_data[0]);
    CH_ASSERT(*al1->last == test_data[0]);
    CH_ASSERT(al1->_array_backing_count == 1);
    CH_ASSERT(al1->_array_backing->size == 1);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 1);
    CH_ASSERT(al1->size == 1);

    ch_vector_t* al2 = ch_vector_ch_word_new(0,cmp_ch_word);
    CH_ASSERT(al2 != al1);
    CH_ASSERT(!al1->eq(al1,al2));
    CH_ASSERT(al2->push_front(al2, test_data[0]));

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al2->eq(al2,al1));
    CH_ASSERT(al1->first != al2->first);
    al1->delete(al1);
    al2->delete(al2);
    return result;
}



/* Insrt an element into an array of size 1*/
static ch_word test4_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_vector_t* al1 = ch_vector_ch_word_new(1,cmp_ch_word);
    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end != al1->last);
    CH_ASSERT(*al1->first == test_data[0]);
    CH_ASSERT(*al1->last == test_data[0]);
    CH_ASSERT(al1->_array_backing_count == 1);
    CH_ASSERT(al1->_array_backing->size == 1);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 1);
    CH_ASSERT(al1->size == 1);

    ch_vector_t* al2 = ch_vector_ch_word_new(1,cmp_ch_word);
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
static ch_word test5_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_vector_t* al1 = ch_vector_ch_word_new(10,cmp_ch_word);
    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end != al1->last);
    CH_ASSERT(*al1->first == test_data[0]);
    CH_ASSERT(*al1->last == test_data[0]);
    CH_ASSERT(al1->_array_backing_count == 1);
    CH_ASSERT(al1->_array_backing->size == 10);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 1);
    CH_ASSERT(al1->size == 10);

    ch_vector_t* al2 = ch_vector_ch_word_new(10,cmp_ch_word);
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
static ch_word test6_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->push_back(al1, test_data[1]));
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last != al1->first);
    CH_ASSERT(al1->end != al1->last);
    CH_ASSERT(*al1->first == test_data[0]);
    CH_ASSERT(*al1->last == test_data[1]);
    CH_ASSERT(al1->_array_backing_count == 2);
    CH_ASSERT(al1->_array_backing->size == 2);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 2);
    CH_ASSERT(al1->size == 2);

    ch_vector_t* al2 = ch_vector_ch_word_new(1,cmp_ch_word);
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
static ch_word test7_ch_word(ch_word* test_data)
{
    ch_word result = 1;
    ch_word* rmv_1 = NULL;
    ch_word* rmv_2 = NULL;

    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    ch_vector_t* al2 = ch_vector_ch_word_new(2,cmp_ch_word);

    /* Make sure that this works a few times over */
    for(ch_word i = 0; i < 10; i++){

        CH_ASSERT(al1->push_back(al1, test_data[0]));
        CH_ASSERT(al1->push_back(al1, test_data[1]));
        CH_ASSERT(al1->first == al1->_array_backing->first);

        CH_ASSERT(al2->push_front(al2, test_data[1]));
        CH_ASSERT(al2->push_front(al2, test_data[0]));

        rmv_1 = al1->first;
        rmv_2 = al2->first;
        CH_ASSERT(*al1->remove(al1,rmv_1) == test_data[1]);
        CH_ASSERT(*al2->remove(al2,rmv_2) == test_data[1]);

        CH_ASSERT(al1->last == al1->first);
        CH_ASSERT(al1->end != al1->last);
        CH_ASSERT(*al1->first == test_data[1]);
        CH_ASSERT(*al1->last == test_data[1]);
        CH_ASSERT(al1->_array_backing_count == 1);
        CH_ASSERT(al1->_array_backing->size == 2);
        CH_ASSERT(al1->count == 1);
        CH_ASSERT(al1->size == 2);

        CH_ASSERT(al2->last == al2->first);
        CH_ASSERT(al2->end != al2->last);
        CH_ASSERT(*al2->first == test_data[1]);
        CH_ASSERT(*al2->last == test_data[1]);
        CH_ASSERT(al2->_array_backing_count == 1);
        CH_ASSERT(al2->_array_backing->size == 2);
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
        CH_ASSERT(al1->_array_backing->size == 2);
        CH_ASSERT(al1->count == 0);
        CH_ASSERT(al1->size == 2);

        CH_ASSERT(al2->last == al2->first);
        CH_ASSERT(al2->end == al2->last);
        CH_ASSERT(al2->_array_backing_count == 0);
        CH_ASSERT(al2->_array_backing->size == 2);
        CH_ASSERT(al2->count == 0);
        CH_ASSERT(al2->size == 2);

        CH_ASSERT(al1->eq(al1,al2));
        CH_ASSERT(al2->eq(al2,al1));
    }


    al1->delete(al1);
    al2->delete(al2);
    return result;
}


static ch_word test8_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    (void)test_data;
    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    ch_vector_t* al2 = ch_vector_ch_word_new(0,cmp_ch_word);

    al1->sort(al1);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing->first == NULL);
    CH_ASSERT(al1->first == NULL);
    CH_ASSERT(al1->last == NULL);
    CH_ASSERT(al1->end == NULL);
    CH_ASSERT(al1->_array_backing_count == 0);
    CH_ASSERT(al1->_array_backing->size == 0);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 0);
    CH_ASSERT(al1->size == 0);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);
    return result;
}

static ch_word test9_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    ch_vector_t* al2 = ch_vector_ch_word_new(0,cmp_ch_word);

    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al2->push_front(al2, test_data[0]));

    al1->sort(al1);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last == al1->last);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_count == 1);
    CH_ASSERT(al1->_array_backing->size == 1);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 1);
    CH_ASSERT(al1->size == 1);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));

    al1->delete(al1);
    al2->delete(al2);
    return result;
}

static ch_word test10_ch_word(ch_word* test_data)
{
    ch_word result = 1;
    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    ch_vector_t* al2 = ch_vector_ch_word_new(0,cmp_ch_word);

    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->push_back(al1, test_data[1]));
    CH_ASSERT(al2->push_front(al2, test_data[0]));
    CH_ASSERT(al2->push_front(al2, test_data[1]));

    al1->sort(al1);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last == al1->last);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_count == 2);
    CH_ASSERT(al1->_array_backing->size == 2);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 2);
    CH_ASSERT(al1->size == 2);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));

    al1->delete(al1);
    al2->delete(al2);
    return result;
}

static ch_word test11_ch_word(ch_word* test_data, ch_word* test_result)
{
    ch_word result = 1;
    /* Test the initial conditions and equality of new (empty) array lists*/
    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    ch_vector_t* al2 = ch_vector_ch_word_new(0,cmp_ch_word);

    for(ch_word i = 0; i < 10; i++){
        al1->push_back_carray(al1,test_data, 15);
        al1->sort(al1);
    }

    al2->push_back_carray(al2,test_result, 150);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last == al1->last);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_count == 150);
    CH_ASSERT(al1->_array_backing->size == 256);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 150);
    CH_ASSERT(al1->size == 256);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));

    al1->delete(al1);
    al2->delete(al2);
    return result;
}


static ch_word test12_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);

    CH_ASSERT(al1->find(al1,al1->first, al1->end, test_data[1]) == NULL);


    al1->delete(al1);

    return result;
}


static ch_word test13_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    al1->push_back_carray(al1, test_data, 3);

    CH_ASSERT(*al1->find(al1,al1->first, al1->end, test_data[1]) == test_data[1]);

    al1->delete(al1);

    return result;
}

static ch_word test14_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    al1->push_back_carray(al1, test_data, 3);

    CH_ASSERT(al1->find(al1,al1->first, al1->end, 42) == NULL);
    CH_ASSERT(al1->find(al1,al1->end, al1->end, 42) == NULL);
    CH_ASSERT(al1->find(al1,al1->first, al1->first + 1, test_data[1]) == NULL);
    CH_ASSERT(al1->find(al1,al1->first + 2, al1->first + 2, test_data[2]) == NULL);
    CH_ASSERT(*al1->find(al1,al1->first, al1->first + 2, test_data[1]) == test_data[1]);

    al1->delete(al1);

    return result;
}


static ch_word test15_ch_word(ch_word* test_data, ch_word* test_result)
{
    ch_word result = 1;
    /* Test the initial conditions and equality of new (empty) array lists*/
    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    ch_vector_t* al2 = ch_vector_ch_word_new(0,cmp_ch_word);

    for(ch_word i = 0; i < 10; i++){
        al1->push_back_carray(al1,test_data, 15);
        al1->sort_reverse(al1);
    }

    al2->push_back_carray(al2,test_result, 150);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last == al1->last);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_count == 150);
    CH_ASSERT(al1->_array_backing->size == 256);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 150);
    CH_ASSERT(al1->size == 256);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));

    al1->delete(al1);
    al2->delete(al2);
    return result;
}

/* Found a bug in the equaility operator. This should stimulate it*/
static ch_word test16_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_vector_t* al1 = ch_vector_ch_word_new(0,cmp_ch_word);
    CH_ASSERT(al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing->first);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end != al1->last);
    CH_ASSERT(*al1->first == test_data[0]);
    CH_ASSERT(*al1->last == test_data[0]);
    CH_ASSERT(al1->_array_backing_count == 1);
    CH_ASSERT(al1->_array_backing->size == 1);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->count == 1);
    CH_ASSERT(al1->size == 1);

    ch_vector_t* al2 = ch_vector_ch_word_new(0,cmp_ch_word);
    CH_ASSERT(!al1->eq(al1,al2));
    CH_ASSERT(al2->push_front(al2, test_data[1]));

    CH_ASSERT(al2 != al1);

    CH_ASSERT(!al1->eq(al1,al2));
    CH_ASSERT(!al2->eq(al2,al1));
    CH_ASSERT(al1->first != al2->first);
    al1->delete(al1);
    al2->delete(al2);
    return result;
}



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    ch_word test_array[15] = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};
    ch_word test_array_sorted[150] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9};
    ch_word test_array_rsorted[150] = {9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,8,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0};

    ch_word test_result = 0;
    ch_word result = 0;

    printf("CH Data Structures: Vector Test 01: ");  printf("%s", (test_result = test1_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 02: ");  printf("%s", (test_result = test2_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 03: ");  printf("%s", (test_result = test3_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 04: ");  printf("%s", (test_result = test4_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 05: ");  printf("%s", (test_result = test5_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 06: ");  printf("%s", (test_result = test6_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 07: ");  printf("%s", (test_result = test7_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 08: ");  printf("%s", (test_result = test8_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 09: ");  printf("%s", (test_result = test9_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 10: ");  printf("%s", (test_result = test10_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 11: ");  printf("%s", (test_result = test11_ch_word(test_array, test_array_sorted)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 12: ");  printf("%s", (test_result = test12_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 13: ");  printf("%s", (test_result = test13_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 14: ");  printf("%s", (test_result = test14_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 15: ");  printf("%s", (test_result = test15_ch_word(test_array, test_array_rsorted)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Vector Test 16: ");  printf("%s", (test_result = test16_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;

    return result;
}


