
/**
 * @file stack.h
 * @author Micah Villmow
 * @brief a reuseable stack class that takes any object
 *
 * the stack class offers two flavors, a stacklist and
 * a stackvector, depending on which underlying structure
 * the programmer prefers
 */
#ifndef STACK_H_
#    define STACK_H_
#include "gen/data_types.h"
#include "gen/gen_macros.h"
#include "gen/function_macros.h"
#include "gen/function_signatures.h"

/**
 * @struct StackList
 * @brief The stack object that handles the objects
 * and is based on a list generic data structure
 */
#ifndef STACKLIST_DEFINED
#define STACKLIST_DEFINED
typedef struct _stack_list StackList;
#endif

/**
 * @brief the iterator object that helps
 * traverse the stack
 */
#ifndef STACKLIST_ITER_DEFINED
#define STACKLIST_ITER_DEFINED
typedef struct _ptr_iter ITER(StackList);
#endif
COMMON_FUNCTION_PROTOTYPES(StackList)
/**
 * @fn int8_t push_StackList(StackList*, void*, size_t, int8_t)
 * @param stack the stack to push an item onto
 * @param obj the obj to push onto the stack
 * @param objsize the size of the object
 * @param flag the flag for this specific push
 * @return 0 on success, non-zero on error
 *
 * @brief pushes an item onto the stack
 */
int8_t push_StackList(StackList * stack, void *obj, size_t objsize,
                             int8_t flag);

#define convert_StackList(A,B,C,D) push_StackList((A),(B),(C),(D))

/**
 * @fn int8_t pop_StackList(StackList*)
 * @param stack the stack to remove the top item from
 * @return 0 on success, non-zero on failure
 *
 * @brief pops the first item off the stack
 */
int8_t pop_StackList(StackList * stack);

/**
 * @fn void *top_StackList(StackList*)
 * @param stack the stack to return the top object from
 * @return the object on success, NULL on failure
 *
 * @brief returns a pointer to the top of the stack so it
 * can be easily referenced
 */
void *top_StackList(StackList * stack);

create_iter_prototypes(StackList)


/* beginning of vector based deque functions */
#ifndef STACKVECTOR_DEFINED
#define STACKVECTOR_DEFINED
typedef struct _array_struct StackVector;
#endif

#ifndef STACKVECTOR_ITER_DEFINED
#define STACKVECTOR_ITER_DEFINED
typedef struct _array_iter ITER(StackVector);
#endif

COMMON_FUNCTION_PROTOTYPES(StackVector)
int8_t pop_StackVector(StackVector *stack);
void * top_StackVector(StackVector *stack);
int8_t push_StackVector(StackVector *stack, void *obj, size_t objsize, int flag);
int8_t resize_StackVector(StackVector *stack,size_t size);

#define convert_StackVector(A,B,C,D)	push_StackVector((A),(B),(C),(D))


create_iter_prototypes(StackVector)
#endif
