
/**
 * @file deque.h
 * @author Micah Villmow
 * @brief a resuable deque class that takes any object
 *
 */
#ifndef QUEUE_H_
#    define QUEUE_H_
#include "gen/data_types.h"
#include "gen/gen_macros.h"
#include "gen/function_macros.h"
#include "gen/function_signatures.h"


/**
 * @struct DequeList
 * @brief the deque object that handles the objects
 * and is based on a generic list data structure
 */
#ifndef DEQUELIST_DEFINED
#define DEQUELIST_DEFINED	
typedef struct _ptr_struct DequeList;
#endif

/**
 * @brief the iterator object that helps
 * traverse the deque
 */
#ifndef DEQUELIST_ITER_DEFINED
#define DEQUELIST_ITER_DEFINED
typedef struct _ptr_iter ITER(DequeList);
#endif

COMMON_FUNCTION_PROTOTYPES(DequeList)

/**
 * @fn int8_t pop_front_DequeList()
 * @param deque the deque to remove the front item from
 * @return 0 on success, non-zero on failure
 * @brief removes the front item from the deque
 */
 int8_t pop_front_DequeList(DequeList * deque);
/**
 * @fn int8_t pop_back_DequeList()
 * @param deque the deque to remove the front item from
 * @return 0 on success, non-zero on failure
 * @brief removes the front item from the deque
 */
 int8_t pop_back_DequeList(DequeList * deque);

/**
 * @fn void* front_DequeList()
 * @param deque the deque to get the first item of
 * @return a pointer to the front object in the stack or NULL on failure
 * @brief returns a pointer to the front object so that it can be
 * easily referenced
 */
 void *front_DequeList(DequeList * deque);

 /**
  * @fn void* back_DequeList()
  * @param deque the deque to get the last item of
  * @return a pointer to the last object in the deque or NULL on failure
  * @brief returns a pointer to the last object so that it can be
  * be easily referenced
  */
 void *back_DequeList(DequeList *deque);

/**
 * @fn int8_t push_back_DequeList()
 * @param deque the deque to push an item into
 * @param obj the object to add to the deque
 * @param objsize the size of the object to add
 * @param flag the flag for the object
 * @return 0 on success, non-zero on error
 *
 * @brief pushes an item onto the end of a deque
 */
 int8_t push_back_DequeList(DequeList * deque, void *obj, size_t objsize,
                         int flag);
int8_t convert_DequeList(DequeList *deque, void *obj, size_t objsize, int flag);
/**
 * @fn int8_t push_front_DequeList()
 * @param deque the deque to push an item into
 * @param obj the object to add to the deque
 * @param objsize the size of the object to add
 * @param flag the flag for the object
 * @return 0 on success, non-zero on error
 *
 * @brief pushes an item onto the end of a deque
 */
 int8_t push_front_DequeList(DequeList * deque, void *obj, size_t objsize,
                         int flag);



create_iter_prototypes(DequeList)

/* beginning of vector based deque functions */
#ifndef DEQUEVECTOR_DEFINED
#define DEQUEVECTOR_DEFINED
typedef struct _array_struct DequeVector;
#endif

#ifndef DEQUEVECTOR_ITER_DEFINED
#define DEQUEVECTOR_ITER_DEFINED
typedef struct _array_iter ITER(DequeVector);
#endif
COMMON_FUNCTION_PROTOTYPES(DequeVector)

int8_t pop_front_DequeVector(DequeVector *deque);
int8_t pop_back_DequeVector(DequeVector *deque);
void * front_DequeVector(DequeVector *deque);
void * back_DequeVector(DequeVector *deque);
int8_t push_front_DequeVector(DequeVector *deque, void *obj, size_t objsize, int flag);
int8_t push_back_DequeVector(DequeVector *deque, void *obj, size_t objsize, int flag);
int8_t resize_DequeVector(DequeVector *deque,size_t size);

#define convert_DequeVector(A,B,C,D)	push_DequeVector((A),(B),(C),(D))

create_iter_prototypes(DequeVector)

#endif
