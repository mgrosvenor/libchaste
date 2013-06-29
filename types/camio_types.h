/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 */


#ifndef CAMIO_TYPES_H_
#define CAMIO_TYPES_H_

#include <stdint.h>
#include "../istreams/camio_istream.h"
#include "../ostreams/camio_ostream.h"


typedef enum {
    CAMIO_NO_TYPE = 0,
    CAMIO_BOOL,
    CAMIO_UINT64,
    CAMIO_INT64,
    CAMIO_STRING,
    CAMIO_DOUBLE,
    CAMIO_BOOLS,     //List types
    CAMIO_UINT64S,
    CAMIO_INT64S,
    CAMIO_STRINGS,
    CAMIO_DOUBLES,
    CAMIO_ISTREAM,   //Exotics
    CAMIO_OSTERAM

} camio_types_e;



#define camio_list_struct_declare(type,name)          \
    typedef struct {                                 \
    uint64_t size;                                   \
    uint64_t count;                                  \
    type* items;                                     \
} camio_##name##_list_t


#define camio_list_t(name)                            \
    camio_##name##_list_t


#define camio_list_init_declare(type, name)            \
    void camio_##name##_list_init(void* list, uint64_t size)

#define camio_list_init(name, list, size)                \
    camio_##name##_list_init(list, size)


#define camio_list_add_declare(type, name)            \
    void camio_##name##_list_add(void* list, type item)

#define camio_list_add(name,list,value)               \
    camio_##name##_list_add(list, value)




#define fep2_list(type, name)               \
    camio_list_struct_declare(type,name);    \
    camio_list_init_declare(type,name);      \
    camio_list_add_declare(type,name);       \


//Define them
fep2_list(int, bool);
fep2_list(char*, string);
fep2_list(uint64_t, uint64 );
fep2_list(int64_t, int64);
fep2_list(double, double);
fep2_list(camio_istream_t*, istream);
fep2_list(camio_ostream_t*, ostream);

#endif /* CAMIO_TYPES_H_ */
