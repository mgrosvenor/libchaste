
/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file access_macros.h
 * @author Micah Villmow
 * @brief a bunch of macros that help with access into
 * a structure
 */
#ifndef ACCESS_MACROS_H_
#    define ACCESS_MACROS_H_
/**
 * @brief flag to pass in if the data to be stored
 * is statically allocated.
 *
 * If the data passed in to a function is statically
 * allocated, then the function will make a copy of
 * the data and store that data instead.
 */
#    ifndef STATIC
#        define STATIC  (0x2)
#    endif

/**
 * @brief flag to pass in if the data to be stored
 * is dynamically allocated.
 *
 * If the data passed in to a function is created dynamically,
 * then the function will not make a copy and instead just
 * point to the dynamically allocated memory.
 */
#    ifndef DYNAMIC
#        define DYNAMIC (0x01)
#    endif
/**
 * @brief Initial number of nodes in the extra node list
 *
 * Each of the pointer based classes has an initial number
 * of objects that they use to minimize the amount of individual
 * allocations that are needed. This value is used to establish
 * the initial size of this list.
 */
#    ifndef INITIAL_SIZE
#        define INITIAL_SIZE 256
#    endif
/**
 * @brief flag to specify to free dynamically allocated
 * objects
 */
#    ifndef FREEOBJ
#        define FREEOBJ 0x1
#    endif
/**
 * @brief flag to specify not to free dynamically allocated objects
 */
#    ifndef NOFREE
#        define NOFREE	0x2
#    endif
/**
 * @brief macro to make accessing the tail pointer faster
 */
#    ifndef T
#        define T(X)	((X)->tail)
#    endif
/**
 * @brief macro to make accessing the head pointer faster
 */
#    ifndef H
#        define H(X)	((X)->head)
#    endif
/**
 * @brief index to pointer array for left child
 */
#    define LEFT 	0x00
/**
 * @brief index to pointer array for right child
 */
#    define RIGHT   0x01

/**
 * @brief macro for the previous node
 */
#    ifndef P
#        define P(X)	L(X)
#    endif
/**
 * @brief macro for the left child
 */
#    ifndef L
#        define L(X)	((X)->ptr[LEFT])
#    endif
/**
 * @brief macro for the flags variable
 */
#define flag(X)	((X)->flags)

/**
 * @brief macro for the next node
 */
#    ifndef N
#        define N(X)	R(X)
#    endif
/**
 * @brief macro for the right child
 */
#    ifndef R
#        define R(X)	((X)->ptr[RIGHT])
#    endif
/**
 * @brief macro for the base pointer
 */
#    ifndef B
#        define B(X)	((X)->base)
#    endif
/**
 * @brief macro for the structure size variable
 */
#    ifndef S
#        define S(X)	((X)->size)
#    endif
/**
 * @brief macro for the structure capacity variable
 */
#    ifndef C
#        define C(X)	((X)->capacity)
#    endif
/**
 * @brief macro for the object size variable
 */
#    ifndef O
#        define O(X)	((X)->objsize)
#    endif

#ifndef M
#define M(X)		((X)->mem)
#endif
/**
 * @brief macro to get the free list pointer
 */
#    ifndef FL
#        define FL(X)	((X)->free_list)
#    endif
/**
 * @brief macro to get to the object of the pointer based iterator
 */
#    define ITERLIST_OBJ(X)  	((X)->ptr->objptr)
/**
 * @brief macro to move the pointer based iterator to the next item
 */
#    define ITERLIST_NEXT(X) 	((X)->ptr = R((X)->ptr))
/**
 * @brief macro to move the pointer based iterator to the prev item
 */
#    define ITERLIST_PREV(X) 	((X)->ptr = L((X)->ptr))
/**
 * @brief macro to get the object of the array based pointer
 */
#define ITERARRAY_OBJ(X)	((X)->ptr)
/**
 * @brief macro to mave the array based iterator to the next item
 */
#define ITERARRAY_NEXT(X)	((X)->ptr = (((char*)(X)->ptr) + O((X)->parent)))
/**
 * @brief macro to move the array based iterator to the prev item
 */
#define ITERARRAY_PREV(X)	((X)->ptr = (((char*)(X)->ptr) - O((X)->parent)))
/**
 * @brief macro to move the iterator to the head
 */
#    define ITER_HEAD(X)	((X)->ptr = H((X)->parent))
/**
 * @brief macro to move the iterator to the tail
 */
#    define ITER_TAIL(X)	((X)->ptr = T((X)->parent))


#endif
