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


    (void)test_data;
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


    (void)test_data;
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


    (void)test_data;
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

    (void)test_data;
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

    (void)test_data;
    ch_array_list_i64_t* al1 = ch_array_list_i64_new(0,cmp_i64);
    CH_ASSERT(rmv_1 = al1->push_back(al1, test_data[0]));
    CH_ASSERT(al1->push_back(al1, test_data[1]));
    CH_ASSERT(al1->first == al1->_array_backing);

    ch_array_list_i64_t* al2 = ch_array_list_i64_new(2,cmp_i64);
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


    al1->delete(al1);
    al2->delete(al2);
    return result;
}



//
//
//void test3()
//{
//    ch_array_list_i64_t* al = ch_array_list_i64_new(10,cmp);
//    for(int i = 0; i < 11; i++){
//        al->push_back(al,0xFF - i);
//    }
//    dump_array(al);
//    al->delete(al);
//}
//
//
//void test4()
//{
//    ch_array_list_i64_t* al = ch_array_list_i64_new(10,cmp);
//    for(int i = 0; i < 15; i++){
//        al->push_back(al,0xFF - i);
//    }
//
//    al->remove(al,al->first);
//
//    dump_array(al);
//    al->delete(al);
//}
//
//
//void test5()
//{
//    ch_array_list_i64_t* al = ch_array_list_i64_new(10,cmp);
//    for(int i = 0; i < 10; i++){
//        al->push_back(al,0xFF - i);
//    }
//
//    for(int i = 0; i < 15; i++){
//        al->remove(al,al->first);
//    }
//
//    dump_array(al);
//    al->delete(al);
//}
//
//
//void test6()
//{
//    ch_array_list_i64_t* al = ch_array_list_i64_new(10,cmp);
//    for(int i = 0; i < 11; i++){
//        al->push_front(al,0xFF - i);
//    }
//    dump_array(al);
//    al->delete(al);
//}
//
//
//void test7()
//{
//    ch_array_list_i64_t* al = ch_array_list_i64_new(10,cmp);
//    for(int i = 0; i < 11; i++){
//        al->push_front(al,0xFF - i);
//    }
//
//    al->resize(al,5);
//
//    dump_array(al);
//
//    al->delete(al);
//}
//
//void test8()
//{
//    ch_array_list_i64_t* al = ch_array_list_i64_new(10,cmp);
//    for(int i = 0; i < 10; i++){
//        al->push_back(al,0xFF - i);
//    }
//
//    dump_array(al);
//    for(int i = -1; i > -11; i--){
//        printf("%li,", *al->off(al,i));
//    }
//
//    al->delete(al);
//}
//
//
//void test9()
//{
//    ch_array_list_i64_t* al = ch_array_list_i64_new(100,cmp);
//    al->push_back(al,4);
//    al->push_back(al,1);
//    al->push_back(al,3);
//    al->push_back(al,6);
//    al->push_back(al,5);
//    al->push_back(al,8);
//    al->push_back(al,4);
//    al->push_back(al,1);
//    al->push_back(al,3);
//    al->push_back(al,6);
//    al->push_back(al,5);
//    al->push_back(al,8);
//    al->push_back(al,4);
//    al->push_back(al,1);
//    al->push_back(al,3);
//    al->push_back(al,6);
//    al->push_back(al,5);
//    al->push_back(al,8);
//    al->push_back(al,4);
//    al->push_back(al,1);
//    al->push_back(al,3);
//    al->push_back(al,6);
//    al->push_back(al,5);
//    al->push_back(al,8);
//    al->push_back(al,4);
//    al->push_back(al,1);
//    al->push_back(al,3);
//    al->push_back(al,6);
//    al->push_back(al,5);
//    al->push_back(al,8);
//    al->push_back(al,4);
//    al->push_back(al,1);
//    al->push_back(al,3);
//    al->push_back(al,6);
//    al->push_back(al,5);
//    al->push_back(al,8);
//    al->push_back(al,4);
//    al->push_back(al,1);
//    al->push_back(al,3);
//    al->push_back(al,6);
//    al->push_back(al,5);
//    al->push_back(al,8);
//    al->push_back(al,4);
//    al->push_back(al,1);
//    al->push_back(al,3);
//    al->push_back(al,6);
//    al->push_back(al,5);
//    al->push_back(al,8);
//
//
//
//    dump_array(al);
//    al->sort_reverse(al);
//    dump_array(al);
//    al->delete(al);
//}
//
//void test10()
//{
//    ch_array_list_i64_t* al = ch_array_list_i64_new(0,cmp);
//    for(int i = 0; i < 10; i++){
//        al->push_back(al,0xFF - i);
//    }
//
//    dump_array(al);
//    al->delete(al);
//}



int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    i64 test_array[15] = {8,5,1,3,4,6,7,9,7,1,6,1,0,1,6};

    printf("CH Data Structures: Array List Test 1: ");  printf("%s", test1_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 2: ");  printf("%s", test2_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 3: ");  printf("%s", test3_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 4: ");  printf("%s", test4_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 5: ");  printf("%s", test5_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 6: ");  printf("%s", test6_i64(test_array) ? "PASS\n" : "FAIL\n");
    printf("CH Data Structures: Array List Test 7: ");  printf("%s", test7_i64(test_array) ? "PASS\n" : "FAIL\n");
    //    printf("test 3\n"); test3(test_array);
//    printf("test 4\n"); test4(test_array);
//    printf("test 5\n"); test5(test_array);
//    printf("test 6\n"); test6(test_array);
//    printf("test 7\n"); test7(test_array);
//    printf("test 8\n"); test8(test_array);
//    printf("test 9\n"); test9(test_array);
//    printf("test 10\n"); test10(test_array);



    return 0;
}


