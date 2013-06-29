/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 */

#include "camio_types.h"
#include "../errors/camio_errors.h"
#include "malloc.h"


#define camio_list_add_define(type, name)                                \
void camio_##name##_list_add(void* listv, type item){                    \
    camio_##name##_list_t* list = (camio_##name##_list_t*)listv;          \
    if(list->count == list->size){                                      \
        list->size *= 2;                                                \
        if(!realloc(list->items,list->size)){                           \
            eprintf_exit( "No memory available for reallocation to size %lu\n", list->count);\
        }                                                               \
    }                                                                   \
                                                                        \
    list->items[list->count] = item;                                    \
    list->count++;                                                      \
}


#define camio_list_init_define(type, name)                               \
void camio_##name##_list_init(void* listv, uint64_t size){               \
    camio_##name##_list_t* list = (camio_##name##_list_t*)listv;          \
    list->size = size;                                                  \
    if(! (list->items = malloc(list->size * sizeof(type))) ) {          \
        eprintf_exit( "No memory available for initialization to size %lu\n", list->count);\
    }                                                                   \
    list->count = 0;                                                    \
}


camio_list_add_define(int,bool);
camio_list_init_define(int, bool);

camio_list_add_define(char*,string);
camio_list_init_define(char*,string);

camio_list_add_define(uint64_t,uint64);
camio_list_init_define(uint64_t, uint64);

camio_list_add_define(int64_t,int64);
camio_list_init_define(int64_t, int64);

camio_list_add_define(double,double);
camio_list_init_define(double, double);

camio_list_add_define(camio_istream_t*,istream);
camio_list_init_define(camio_istream_t*, istream);

camio_list_add_define(camio_ostream_t*,ostream);
camio_list_init_define(camio_ostream_t*, ostream);

