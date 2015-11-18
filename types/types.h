/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 */


#ifndef CH_TYPES_H_
#define CH_TYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

//#include "../deps/libcgen/vector.h"

//Unsigned types, use these sparingly
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
//typedef uint64_t    u64; //Try to avoid this unless absolutely necessary. Too many bugs are caused by int/uint mixups
typedef unsigned long long    u64; //Try to avoid this unless absolutely necessary. Too many bugs are caused by int/uint mixups

//Signed types, use these sparingly
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef long long   i64; //Try to avoid this, use ch_word.


//CH Library Preferred Types
typedef size_t  ch_machine; //Use this sparingly, only when it really matters that the int size is the same as the word size
typedef i8      ch_char;
typedef u8      ch_byte;
typedef long long int     ch_word; //Singed int chosen specifically to avoid underrun and signed/unsigned comparisons
typedef bool    ch_bool; //Give the compiler tons of freedom to decide what to do here
typedef double	ch_float; //Again we preference 64bit machines explicitly

//Please avoid using char*. CH strings are a thin wrapper, that support easy concatenation and other functions
typedef struct {
    char* cstr; //Can be NULL or point to a const or non-const memory area
    int is_const; //Can be -1 (unknown), 0 (can call free) or 1 (cannot call free)
    i64 slen; //A valid string len is always -1 (unknown), or < mlen
    i64 mlen; //A valid memory len is always -1 (unknown), or > slen
} ch_str;

typedef char* ch_cstr; //Use this sparingly, only when it really matters that you are backwards compatible with c-strings
typedef const char* ch_ccstr; //Use this sparingly as well

//#define cstr(bstr) ((char*)(bstr->data))

typedef enum {
    CH_NO_TYPE = 0,
    CH_BOOL,
    CH_UINT64,
    CH_HEX,
    CH_INT64,
    CH_STRING,
    CH_DOUBLE,
    CH_BOOLS,     //Vector types
    CH_UINT64S,
    CH_INT64S,
    CH_HEXS,
    CH_STRINGS,
    CH_DOUBLES,
} ch_types_e;

ch_word is_vector(ch_word type);

typedef int (*cmp_void_f)(const void*, const void*);



#endif /* CH_TYPES_H_ */
