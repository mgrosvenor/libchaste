// CamIO 2: demo_array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../data_structs/array_std.h"
#include "../utils/util.h"

#include <stdio.h>



#define ch_array_t ch_array_ch_word_t

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


void dump_array_ch_word(ch_array_t* al)
{
    printf("There are %li slots in the array\n", al->size);
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
    ch_array_t* al1 = ch_array_ch_word_new(0,cmp_ch_word);
    CH_ASSERT(al1->_array_backing == NULL);
    CH_ASSERT(al1->first == NULL);
    CH_ASSERT(al1->last == NULL);
    CH_ASSERT(al1->end == NULL);
    CH_ASSERT(al1->_array_backing_size == 0);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->size == 0);

    ch_array_t* al2 = ch_array_ch_word_new(0,cmp_ch_word);
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
    ch_array_t* al1 = ch_array_ch_word_new(1,cmp_ch_word);
    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_size == 1);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->size == 1);

    ch_array_t* al2 = ch_array_ch_word_new(1,cmp_ch_word);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);

    return result;
}

/* Get an element at 0/10 from array of size 0. Use both push back and push front*/
static ch_word test3_ch_word(ch_word* test_data)
{

    printf("....\nExpected output:\n");
    printf("------------------------------------------------------------\n");
    printf("Index (0/10) is out of the valid range [0,-1]\n");
    printf("Index (10) is out of the valid range [0,-1]\n");
    printf("------------------------------------------------------------\n");
    printf("Actual Output:\n");
    printf("------------------------------------------------------------\n");
    (void)test_data;
    ch_word result = 1;

    ch_word* ptr = NULL;

    ch_array_t* al1 = ch_array_ch_word_new(0,cmp_ch_word);
    CH_ASSERT( (ptr = al1->off(al1, 0)) == NULL);
    CH_ASSERT(al1->_array_backing == NULL);

    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end == al1->last);

    CH_ASSERT(al1->_array_backing_size == 0);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->size == 0);

    ch_array_t* al2 = ch_array_ch_word_new(0,cmp_ch_word);
    CH_ASSERT( (ptr = al1->off(al1, 10)) == NULL );
    CH_ASSERT(ptr == NULL);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al2->eq(al2,al1));
    CH_ASSERT(al1->first == al2->first);
    al1->delete(al1);
    al2->delete(al2);
    printf("------------------------------------------------------------\n");
    printf("CH Data Structures: Array Test 03:");
    return result;
}

/* Get/set an element at 0 from array of size 1.*/
static ch_word test4_ch_word(ch_word* test_data)
{

    ch_word result = 1;

    ch_word* ptr = NULL;

    ch_array_t* al1 = ch_array_ch_word_new(1,cmp_ch_word);
    ch_array_t* al2 = ch_array_ch_word_new(1,cmp_ch_word);
    CH_ASSERT(al2 != al1);

    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->first);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_size == 1);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->size == 1);
    CH_ASSERT(al1->eq(al1,al2));

    CH_ASSERT( (ptr = al1->off(al1, 0)) != NULL);
    *ptr = test_data[0];

    CH_ASSERT(!al1->eq(al1,al2));

    CH_ASSERT( (ptr = al2->off(al2, 0)) != NULL );
    *ptr = test_data[0];

    CH_ASSERT( *al1->first == test_data[0] );
    CH_ASSERT( *al2->first == test_data[0] );

    CH_ASSERT(al2->eq(al2,al1));
    CH_ASSERT(al1->first != al2->first);

    al1->delete(al1);
    al2->delete(al2);

    return result;
}


/* Get/set an element at 5 from array of size 10. Use both positive and negative indexes */
static ch_word test5_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_word* ptr = NULL;

    ch_array_t* al1 = ch_array_ch_word_new(10,cmp_ch_word);
    ch_array_t* al2 = ch_array_ch_word_new(10,cmp_ch_word);
    CH_ASSERT(al2 != al1);

    CH_ASSERT(al1->_array_backing != NULL);
    CH_ASSERT(al1->first == al1->_array_backing);
    CH_ASSERT(al1->last == al1->first + 9);
    CH_ASSERT(al1->end == al1->last + 1);
    CH_ASSERT(al1->_array_backing_size == 10);
    CH_ASSERT(al1->_cmp == cmp_ch_word);
    CH_ASSERT(al1->size == 10);
    CH_ASSERT(al1->eq(al1,al2));

    CH_ASSERT( (ptr = al1->off(al1, 5)) != NULL);
    *ptr = test_data[0];

    CH_ASSERT(!al1->eq(al1,al2));

    CH_ASSERT( (ptr = al2->off(al2, -5)) != NULL );
    *ptr = test_data[0];

    CH_ASSERT( *(al1->first + 5) == test_data[0] );
    CH_ASSERT( *(al2->first + 5) == test_data[0] );

    CH_ASSERT(al2->eq(al2,al1));
    CH_ASSERT(al1->first != al2->first);

    al1->delete(al1);
    al2->delete(al2);

    return result;   return result;
}

/* Sort an empty array size 0 */
static ch_word test6_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    (void)test_data;
    ch_array_t* al1 = ch_array_ch_word_new(0,cmp_ch_word);
    ch_array_t* al2 = ch_array_ch_word_new(0,cmp_ch_word);

    al1->sort(al1);

    CH_ASSERT(al2 != al1);
    CH_ASSERT(al1->_array_backing == NULL);
    CH_ASSERT(al1->first == NULL);
    CH_ASSERT(al1->last == NULL);
    CH_ASSERT(al1->end == NULL);

    CH_ASSERT(al1->_array_backing_size == 0);
    CH_ASSERT(al1->_cmp == cmp_ch_word);

    CH_ASSERT(al1->size == 0);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);
    return result;
}

/* Sort an empty array. Size 10 */
static ch_word test7_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    (void)test_data;
    ch_array_t* al1 = ch_array_ch_word_new(10,cmp_ch_word);
    ch_array_t* al2 = ch_array_ch_word_new(10,cmp_ch_word);

    al1->sort(al1);

    CH_ASSERT(al2 != al1);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);
    return result;
}

/* Sort an array of 10 element. Size 10 */
static ch_word test8_ch_word(ch_word* test_data, ch_word* test_data_sorted)
{
    ch_word result = 1;

    (void)test_data;
    ch_array_t* al1 = ch_array_ch_word_new(10,cmp_ch_word);
    ch_array_t* al2 = ch_array_ch_word_new(10,cmp_ch_word);

    al1->from_carray(al1, test_data,15);
    al1->sort(al1);

    CH_ASSERT(al2 != al1);

    al2->from_carray(al2, test_data_sorted,15);



    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);
    return result;
}

/* Reverse sort an array of 10 elements. Size 10 */
static ch_word test9_ch_word(ch_word* test_data, ch_word* test_data_sorted)
{
    ch_word result = 1;

    (void)test_data;
    ch_array_t* al1 = ch_array_ch_word_new(10,cmp_ch_word);
    ch_array_t* al2 = ch_array_ch_word_new(10,cmp_ch_word);

    al1->from_carray(al1, test_data,15);
    al1->sort_reverse(al1);

    CH_ASSERT(al2 != al1);

    al2->from_carray(al2, test_data_sorted,15);

    CH_ASSERT(al1->eq(al1,al2));
    CH_ASSERT(al1->eq(al2,al1));
    al1->delete(al1);
    al2->delete(al2);
    return result;
}


static ch_word test10_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_array_t* al1 = ch_array_ch_word_new(0,cmp_ch_word);

    CH_ASSERT(al1->find(al1,al1->first, al1->end, test_data[1]) == NULL);


    al1->delete(al1);

    return result;
}


static ch_word test11_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_array_t* al1 = ch_array_ch_word_new(10,cmp_ch_word);
    al1->from_carray(al1, test_data, 3);

    CH_ASSERT(*al1->find(al1,al1->first, al1->end, test_data[1]) == test_data[1]);

    al1->delete(al1);

    return result;
}

static ch_word test12_ch_word(ch_word* test_data)
{
    ch_word result = 1;

    ch_array_t* al1 = ch_array_ch_word_new(10,cmp_ch_word);
    al1->from_carray(al1, test_data, 3);

    CH_ASSERT(al1->find(al1,al1->first, al1->end, 42) == NULL);
    CH_ASSERT(al1->find(al1,al1->end, al1->end, 42) == NULL);
    CH_ASSERT(al1->find(al1,al1->first, al1->first + 1, test_data[1]) == NULL);
    CH_ASSERT(al1->find(al1,al1->first + 2, al1->first + 2, test_data[2]) == NULL);
    CH_ASSERT(*al1->find(al1,al1->first, al1->first + 2, test_data[1]) == test_data[1]);

    al1->delete(al1);

    return result;
}



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    ch_word test_array[15]         = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};
    ch_word test_array_sorted[15]  = {1,1,3,4,5,6,7,7,8,9,0,0,0,0,0};
    ch_word test_array_rsorted[15] = {9,8,7,7,6,5,4,3,1,1,1,1,1,1,0};

    ch_word test_result = 0;
    ch_word result = 0;

    printf("CH Data Structures: Array Test 01: ");  printf("%s", (test_result = test1_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 02: ");  printf("%s", (test_result = test2_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 03: ");  printf("%s", (test_result = test3_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 04: ");  printf("%s", (test_result = test4_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 05: ");  printf("%s", (test_result = test5_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 06: ");  printf("%s", (test_result = test6_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 07: ");  printf("%s", (test_result = test7_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 08: ");  printf("%s", (test_result = test8_ch_word(test_array, test_array_sorted)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 09: ");  printf("%s", (test_result = test9_ch_word(test_array, test_array_rsorted)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 10: ");  printf("%s", (test_result = test10_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 11: ");  printf("%s", (test_result = test11_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;
    printf("CH Data Structures: Array Test 12: ");  printf("%s", (test_result = test12_ch_word(test_array)) ? "PASS\n" : "FAIL\n"); result += !test_result;

    return result;
}


