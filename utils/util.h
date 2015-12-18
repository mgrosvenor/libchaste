/*
 * util.h
 *
 *  Created on: Nov 18, 2012
 *      Author: mgrosvenor
 */

#ifndef CH_UTIL_H_
#define CH_UTIL_H_

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#define eqlikely(x)     (x)

#define MIN(x,y) ( (x) < (y) ?  (x) : (y))
#define MIN3(x,y,z) (MIN(MIN(x,y),z))
#define MAX(x,y) ( (x) > (y) ?  (x) : (y))

#include "../types/types.h"
#include "debug.h"
///**
// * (from Linux kernel source)
// * container_of - cast a member of a structure out to the containing structure
// * @ptr:    the pointer to the member.
// * @type:   the type of the container struct this is embedded in.
// * @member: the name of the member within the struct.
// *
// */
//#define container_of(ptr, type, member) ({          \
//    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
//    (type *)( (char *)__mptr - offsetof(type,member) );})


/*
 * The below version is borrowed from http://stackoverflow.com/questions/10269685/kernels-container-of-any-way-to-make-it-iso-conforming
 * The new one is better because it is iso compatible (apparently)
 */

#ifdef __GNUC__
#define member_type(type, member) __typeof__ (((type *)0)->member)
#else
#define member_type(type, member) const void
#endif

#define container_of(ptr, type, member) ((type *)( \
    (char *)(member_type(type, member) *){ ptr } - offsetof(type, member)))

//#ifndef __clang__
//    #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
//#endif

ch_word next_pow2(ch_word v);

#define CH_ASSERT(p) do { if(!(p)) { fprintf(stdout, "Error in %s: failed assertion \""#p"\" on line %u\n", __FUNCTION__, __LINE__); result = 0; } } while(0)

//Uses integer division to round up
#define round_up( value, nearest) ((( value + nearest -1) / nearest ) * nearest )

#endif /* UTIL_H_ */
