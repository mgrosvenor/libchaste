/*
 * camio_util.h
 *
 *  Created on: Nov 18, 2012
 *      Author: mgrosvenor
 */

#ifndef CAMIO_UTIL_H_
#define CAMIO_UTIL_H_

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#define MIN(x,y) ( (x) < (y) ?  (x) : (y))
#define MAX(x,y) ( (x) > (y) ?  (x) : (y))


/**
 * (from Linux kernel source)
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

#ifndef __clang__
    #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#endif /* CAMIO_UTIL_H_ */
