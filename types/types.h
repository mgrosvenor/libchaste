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
typedef uint64_t    u64; //Try to avoid this unless absolutely necessary. Too many bugs are caused by int/uint mixups

//Signed types, use these sparingly
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64; //Try to avoid this, use ch_word.


//CH Library Preferred Types
typedef size_t  ch_machine; //Use this sparingly, only when it really matters that the int size is the same as the machine size
typedef i8      ch_char;
typedef u8      ch_byte;
typedef i64     ch_word; //Singed int chosen specifically to avoid underrun and signed/unsigned comparisons
typedef bool    ch_bool; //Give the compiler tons of freedom to decide what to do here

//Please avoid using char*. CH strings are a thin wrapper, that support easy concatenation and other functions
typedef struct {
    char* cstr; //Can be NULL or point to a const or non-const memory area
    int is_const; //Can be -1 (unknown), 0 (can call free) or 1 (cannot call free)
    int is_stack; //Can be -1 (unknown), 0 (not stack allocated) or 1 (stack allocated)
    i64 slen; //A valid string len is always -1 (unknown), or < mlen
    i64 mlen; //A valid memory len is always -1 (unknown), or > slen
} ch_str;


//#define cstr(bstr) ((char*)(bstr->data))

typedef enum {
    CH_NO_TYPE = 0,
    CH_BOOL,
    CH_UINT64,
    CH_INT64,
    CH_STRING,
    CH_DOUBLE,
    CH_BOOLS,     //Vector types
    CH_UINT64S,
    CH_INT64S,
    CH_STRINGS,
    CH_DOUBLES,
} ch_types_e;

ch_word is_vector(ch_word type);





#endif /* CH_TYPES_H_ */
