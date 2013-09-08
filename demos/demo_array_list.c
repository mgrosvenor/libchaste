// CamIO 2: demo_array.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include "../data_structs/array_list_std.h"
#include <stdio.h>

#define TYPE int

#define ch_array_t(TYPE) ch_array_##TYPE##_t

ch_word cmp(double lhs, double rhs)
{
    if(lhs < rhs){
        return -1;
    }

    if(lhs > rhs){
        return 1;
    }

    return 0;
}


void dump_array(ch_array_double_t* al)
{
    printf("There are %li items in the list\n", al->count);
    for(double* i = al->first; i < al->end; i = al->next(al, i)){
        printf("%lf,", *i);
    }
    printf("\n");
}


void test1()
{
    ch_array_double_t* al1 = ch_array_double_new(0,cmp);
    dump_array(al1);
    al1->delete(al1);
}



void test2()
{
    ch_array_double_t* al = ch_array_double_new(1,cmp);
    al->push_back(al,0xFF);
    dump_array(al);
    al->delete(al);
}


void test3()
{
    ch_array_double_t* al = ch_array_double_new(10,cmp);
    for(int i = 0; i < 11; i++){
        al->push_back(al,0xFF - i);
    }
    dump_array(al);
    al->delete(al);
}


void test4()
{
    ch_array_double_t* al = ch_array_double_new(10,cmp);
    for(int i = 0; i < 15; i++){
        al->push_back(al,0xFF - i);
    }

    al->remove(al,al->first);

    dump_array(al);
    al->delete(al);
}


void test5()
{
    ch_array_double_t* al = ch_array_double_new(10,cmp);
    for(int i = 0; i < 10; i++){
        al->push_back(al,0xFF - i);
    }

    for(int i = 0; i < 15; i++){
        al->remove(al,al->first);
    }

    dump_array(al);
    al->delete(al);
}


void test6()
{
    ch_array_double_t* al = ch_array_double_new(10,cmp);
    for(int i = 0; i < 11; i++){
        al->push_front(al,0xFF - i);
    }
    dump_array(al);
    al->delete(al);
}


void test7()
{
    ch_array_double_t* al = ch_array_double_new(10,cmp);
    for(int i = 0; i < 11; i++){
        al->push_front(al,0xFF - i);
    }

    al->resize(al,5);

    dump_array(al);

    al->delete(al);
}

void test8()
{
    ch_array_double_t* al = ch_array_double_new(10,cmp);
    for(int i = 0; i < 10; i++){
        al->push_back(al,0xFF - i);
    }

    dump_array(al);
    for(int i = -1; i > -11; i--){
        printf("%lf,", *al->off(al,i));
    }

    al->delete(al);
}


void test9()
{
    ch_array_double_t* al = ch_array_double_new(100,cmp);
    al->push_back(al,4);
    al->push_back(al,1);
    al->push_back(al,3);
    al->push_back(al,6);
    al->push_back(al,5);
    al->push_back(al,8);
    al->push_back(al,4);
    al->push_back(al,1);
    al->push_back(al,3);
    al->push_back(al,6);
    al->push_back(al,5);
    al->push_back(al,8);
    al->push_back(al,4);
    al->push_back(al,1);
    al->push_back(al,3);
    al->push_back(al,6);
    al->push_back(al,5);
    al->push_back(al,8);
    al->push_back(al,4);
    al->push_back(al,1);
    al->push_back(al,3);
    al->push_back(al,6);
    al->push_back(al,5);
    al->push_back(al,8);
    al->push_back(al,4);
    al->push_back(al,1);
    al->push_back(al,3);
    al->push_back(al,6);
    al->push_back(al,5);
    al->push_back(al,8);
    al->push_back(al,4);
    al->push_back(al,1);
    al->push_back(al,3);
    al->push_back(al,6);
    al->push_back(al,5);
    al->push_back(al,8);
    al->push_back(al,4);
    al->push_back(al,1);
    al->push_back(al,3);
    al->push_back(al,6);
    al->push_back(al,5);
    al->push_back(al,8);
    al->push_back(al,4);
    al->push_back(al,1);
    al->push_back(al,3);
    al->push_back(al,6);
    al->push_back(al,5);
    al->push_back(al,8);



    dump_array(al);
    al->sort_reverse(al);
    dump_array(al);
    al->delete(al);
}


int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

//    printf("test 1\n"); test1();
//    printf("test 2\n"); test2();
//    printf("test 3\n"); test3();
//    printf("test 4\n"); test4();
//    printf("test 5\n"); test5();
//    printf("test 6\n"); test6();
//    printf("test 7\n"); test7();
//    printf("test 8\n"); test8();
    printf("test 9\n"); test9();



    return 0;
}
