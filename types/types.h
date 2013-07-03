/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 */


#ifndef CAMIO_TYPES_H_
#define CAMIO_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

#include "../deps/libbstring/bstrlib.h"
#include "../deps/libbstring/bstraux.h"

//Unsigned types, use these sparingly
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64; //Try to avoid this unless absolutely necessary. Too many bugs are caused by int/uint mixups

//Signed types, use these sparingly
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64; //Try to avoid this, use m6_word.


//M6 Library Preferred Types
typedef size_t  m6_machine; //Use this sparingly, only when it really matters that the int size is the same as the machine size
typedef u8      m6_char;
typedef i64     m6_word; //Singed int chosen specifically to avoid underrun and signed/unsigned comparisons
typedef bool    m6_bool; //Give the compiler tons of freedom to decide what to do here
typedef bstring m6_str;  //Please avoid using char*. M6 uses the bstring library instead. Use char* only when interacting with other APIs
#define cstr(bstr) ((char*)(bstr->data))


















//#include "../istreams/camio_istream.h"
//#include "../ostreams/camio_ostream.h"


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
//fep2_list(camio_istream_t*, istream);
//fep2_list(camio_ostream_t*, ostream);

#endif /* CAMIO_TYPES_H_ */
