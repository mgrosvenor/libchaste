// CamIO 2: demo_array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../data_structs/function_hash_map/function_hash_map.h"
#include "../utils/util.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* key;
    char* value;
} kv;


ch_word func(ch_word value, void* key, ch_word key_size, void* data, ch_word index)
{
    (void)value;
    (void)key;
    (void)key_size;
    (void)data;
    return index;
}



void dump_function_hash_map_i64(ch_function_hash_map* hm)
{
    printf("---------------------------------------------------\n");
    printf("%lli pushes have been made on the function_hash_map\n", hm->count);
    for(ch_function_hash_map_it i = function_hash_map_first(hm); i.key ;  function_hash_map_next(hm, &i)){
        printf("%lli --> %lli\n", *(i64*)i.key, i.value);
    }
    printf("\n");
    printf("---------------------------------------------------\n");
}


static ch_word test1_i64(kv* test_data)
{
    ch_word result = 1;
    (void)test_data;

    //Make an empty function_hash_map
    ch_function_hash_map* hm1 = ch_function_hash_map_new(10,func);
    function_hash_map_delete(hm1);

    return result;
}


static ch_word test2_i64(kv* test_data)
{
    ch_word result = 1;

    //Make an empty function_hash_map
    ch_function_hash_map* hm1 = ch_function_hash_map_new(10,func);

    ch_function_hash_map_it it1 = function_hash_map_push_unsafe_ptr(hm1,test_data[0].key,strlen(test_data[0].key), &test_data[0].value );

    CH_ASSERT(it1.key == test_data[0].key);
    CH_ASSERT(it1.key_size == (ch_word)strlen(test_data[0].key));
    CH_ASSERT(it1.value == 0);


    ch_function_hash_map_it it2 = function_hash_map_get_first(hm1,test_data[0].key, strlen(test_data[0].key) );

    CH_ASSERT(it2.key == test_data[0].key);
    CH_ASSERT(it2.key_size == (ch_word)strlen(test_data[0].key));
    CH_ASSERT(it2.value == 0);

    function_hash_map_delete(hm1);

    return result;
}


static ch_word test3_i64(kv* test_data)
{

    ch_word result = 1;
    (void)test_data;

    //Make an empty function_hash_map
    ch_function_hash_map* hm1 = ch_function_hash_map_new(10,func);

    ch_word results[3] = { 0, 1, 0 };

    for(ch_word i = 0; i < 3; i++){
        //printf("[%li] Pushing %s->%s\n", i, test_data[i].key, test_data[i].value);
        ch_function_hash_map_it it1 = function_hash_map_push_unsafe_ptr(hm1,test_data[i].key,strlen(test_data[i].key), &test_data[i].value );
        ch_function_hash_map_it it2 = function_hash_map_get_first(hm1,test_data[i].key, strlen(test_data[i].key) );
        ch_function_hash_map_it tmp;
        while( (tmp = function_hash_map_get_next(it2)).value ){
            it2 = tmp;
        }

        CH_ASSERT(it1.key == test_data[i].key);
        CH_ASSERT(it1.key_size == (ch_word)strlen(test_data[i].key));
        CH_ASSERT(it1.value == results[i]);

        //printf("[%li] Got %s->%s\n", i, it2.key,*(char**)it2.value );
        CH_ASSERT(it2.key == test_data[i].key);
        CH_ASSERT(it2.key_size == (ch_word)strlen(test_data[i].key));
        CH_ASSERT(it2.value == results[i]);
    }

    function_hash_map_delete(hm1);

    return result;
}



static ch_word test4_i64(kv* test_data)
{
    ch_word result = 1;

    //Make an empty function_hash_map
    ch_function_hash_map* hm1 = ch_function_hash_map_new(10,func);

    uint64_t key = strtoll(test_data[0].key, NULL, 10);
    ch_function_hash_map_it it1 = function_hash_map_push(hm1,&key,sizeof(key), &test_data[0].value );

    CH_ASSERT(it1.key != NULL);
    CH_ASSERT(it1.key_size == sizeof(key));
    CH_ASSERT(*(u64*)it1.key == key);
    CH_ASSERT(it1.value == 0);

    uint64_t key2 = strtoll(test_data[0].key, NULL, 10);
    ch_function_hash_map_it it2 = function_hash_map_get_first(hm1,&key2,sizeof(key2));

    CH_ASSERT(it2.key != NULL);
    CH_ASSERT(it2.key_size == sizeof(key));
    CH_ASSERT(*(u64*)it2.key == key);
    CH_ASSERT(it2.key_size == sizeof(key2));
    CH_ASSERT(it2.value == 0);

    function_hash_map_delete(hm1);

    return result;
}


static ch_word test5_i64(kv* test_data)
{

    ch_word result = 1;
    (void)test_data;

    //Make an empty function_hash_map
    ch_function_hash_map* hm1 = ch_function_hash_map_new(10,func);
    ch_word results[3] = { 0, 1, 0 };

    for(ch_word i = 0; i < 3; i++){
        //printf("[%li] Pushing %s->%s\n", i, test_data[i].key, test_data[i].value);
        uint64_t key = strtoll(test_data[i].key, NULL, 10);

        ch_function_hash_map_it it1 = function_hash_map_push(hm1,&key,sizeof(key), &test_data[i].value );

        uint64_t key2 = strtoll(test_data[i].key, NULL, 10);
        ch_function_hash_map_it it2 = function_hash_map_get_first(hm1,&key2, sizeof(key2) );
        ch_function_hash_map_it tmp;
        while( (tmp = function_hash_map_get_next(it2)).value ){
            it2 = tmp;
        }

        CH_ASSERT(*(u64*)it1.key == key2);
        CH_ASSERT(it1.key_size == sizeof(key2));
        CH_ASSERT(it1.value == results[i]);

        //printf("[%li] Got %s->%s\n", i, it2.key,*(char**)it2.value );
        CH_ASSERT(*(u64*)it2.key == key2);
        CH_ASSERT(it2.key_size == sizeof(key));
        CH_ASSERT(it2.value  == results[i]);
    }

    function_hash_map_delete(hm1);

    return result;
}

//Stimulate a bug when pushing pointers stack pointers in to the hashmap in an unsafe way.
//This will fail when using push_unsafe_ptr
static ch_word test6_i64(kv* test_data)
{

    ch_word result = 1;
    (void)test_data;

    //Make an empty function_hash_map
    ch_function_hash_map* hm1 = ch_function_hash_map_new(10,func);

    for(ch_word i = 0; i < 3; i++){
        //printf("[%li] Pushing %s->%s\n", i, test_data[i].key, test_data[i].value);
        uint64_t key = strtoll(test_data[i].key, NULL, 10);

        function_hash_map_push(hm1,&key,sizeof(key), &test_data[i].value );
    }

	uint64_t key2 = strtoll(test_data[0].key, NULL, 10);
	ch_function_hash_map_it it2 = function_hash_map_get_first(hm1,&key2, sizeof(key2) );

	//printf("[%li] Got %s->%s\n", i, it2.key,*(char**)it2.value );
	CH_ASSERT(*(u64*)it2.key == key2);
	CH_ASSERT(it2.key_size == sizeof(key2));
	CH_ASSERT(it2.value == 1);

	ch_function_hash_map_it tmp;
	while( (tmp = function_hash_map_get_next(it2)).value ){
		it2 = tmp;
	}

	//printf("[%li] Got %s->%s\n", i, it2.key,*(char**)it2.value );
	CH_ASSERT(*(u64*)it2.key == key2);
	CH_ASSERT(it2.key_size == sizeof(key2));
	CH_ASSERT(it2.value == 1);

	uint64_t key3 = strtoll(test_data[2].key, NULL, 10);
	ch_function_hash_map_it it3 = function_hash_map_get_first(hm1,&key3, sizeof(key3) );

	CH_ASSERT(*(u64*)it3.key == key3);
	CH_ASSERT(it3.key_size == sizeof(key3));
	CH_ASSERT(it3.value == 0);

	ch_function_hash_map_it tmp2;
	while( (tmp = function_hash_map_get_next(it2)).value ){
		it2 = tmp2;
	}

	CH_ASSERT(*(u64*)it3.key == key3);
	CH_ASSERT(it3.key_size == sizeof(key3));
	CH_ASSERT(it3.value == 0);

    function_hash_map_delete(hm1);

    return result;
}

//Try out the new first/next functions for iteration
static ch_word test7_i64(kv* test_data)
{

    ch_word result = 1;
    (void)test_data;

    //Make an empty function_hash_map
    ch_function_hash_map* hm1 = ch_function_hash_map_new(10,func);

    for(ch_word i = 0; i < 3; i++){
        //printf("[%li] Pushing %s->%s\n", i, test_data[i].key, test_data[i].value);
        uint64_t key = strtoll(test_data[i].key, NULL, 10);
        function_hash_map_push(hm1,&key,sizeof(key), &test_data[i].value );
    }

    //Items can come out of this in any order, since this is a hash-map
    ch_function_hash_map_it it1 = function_hash_map_first(hm1);
    ch_function_hash_map_it it2 = it1;

    uint64_t key1 = strtoll(test_data[0].key, NULL, 10);
    uint64_t key2 = strtoll(test_data[2].key, NULL, 10);

    CH_ASSERT(*(u64*)it1.key == key1 || *(u64*)it1.key == key2);
    CH_ASSERT(it1.key_size == sizeof(key1) || it1.key_size == sizeof(key2));
    CH_ASSERT(it1.value == 1 || it1.value ==  0 );

    function_hash_map_next(hm1,&it2);
    CH_ASSERT(it2.key);
    CH_ASSERT(*(u64*)it2.key != *(u64*)it1.key);
    CH_ASSERT(it2.key_size == it1.key_size);
    CH_ASSERT(it2.value != it1.value);

    //dump_function_hash_map_i64(hm1);

    return result;


}


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    kv test_data[3] = {
        { "111", "value1" },
        { "111", "value2" },
        { "22", "value2" }
    };


    ch_word test_result = 0;

    printf("CH Data Structures: Generic Function Hash Map Test 01: ");  printf("%s", (test_result = test1_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Function Hash Map Test 02: ");  printf("%s", (test_result = test2_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Function Hash Map Test 03: ");  printf("%s", (test_result = test3_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Function Hash Map Test 04: ");  printf("%s", (test_result = test4_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Function Hash Map Test 05: ");  printf("%s", (test_result = test5_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Function Hash Map Test 06: ");  printf("%s", (test_result = test6_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
    printf("CH Data Structures: Generic Function Hash Map Test 07: ");  printf("%s", (test_result = test7_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Function Hash Map Test 08: ");  printf("%s", (test_result = test8_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Function Hash Map Test 09: ");  printf("%s", (test_result = test9_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Function Hash Map Test 10: ");  printf("%s", (test_result = test10_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Function Hash Map Test 11: ");  printf("%s", (test_result = test11_i64(test_data, test_data_sorted)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Function Hash Map Test 12: ");  printf("%s", (test_result = test12_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Function Hash Map Test 13: ");  printf("%s", (test_result = test13_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Function Hash Map Test 14: ");  printf("%s", (test_result = test14_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;
//    printf("CH Data Structures: Generic Function Hash Map Test 15: ");  printf("%s", (test_result = test16_i64(test_data)) ? "PASS\n" : "FAIL\n"); if(!test_result) return 1;

    return 0;
}



