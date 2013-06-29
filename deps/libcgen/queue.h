
/**
 * @file queue.h
 * @author Micah Villmow
 * @brief a resuable queue class that takes any object
 *
 */
#ifndef QUEUE_H_
#    define QUEUE_H_
#include "gen/data_types.h"
#include "gen/gen_macros.h"
#include "gen/function_macros.h"
#include "gen/function_signatures.h"


/**
 * @struct QueueList
 * @brief the queue object that handles the objects
 * and is based on a generic list data structure
 */
#ifndef QUEUELIST_DEFINED
#define QUEUELIST_DEFINED
typedef struct _ptr_struct QueueList;
#endif

/**
 * @brief the iterator object that helps
 * traverse the queue
 */
#ifndef QUEUELIST_ITER_DEFINED
#define QUEUELIST_ITER_DEFINED	
typedef struct _ptr_iter ITER(QueueList);
#endif

COMMON_FUNCTION_PROTOTYPES(QueueList)

/**
 * @fn int8_t pop_QueueList(QueueList*)
 * @param queue the queue to remove the front item from
 * @return 0 on success, non-zero on failure
 * @brief removes the front item from the queue
 */
 int8_t pop_QueueList(QueueList * queue);

/**
 * @fn void* front_QueueList(QueueList*)
 * @param queue the queue to get the first item of
 * @return a pointer to the front object in the stack or NULL on failure
 * @brief returns a pointer to the front object so that it can be
 * easily referenced
 */
 void *front_QueueList(QueueList * queue);

/**
 * @fn int8_t push_QueueList(QueueList*,void*,size_t,int)
 * @param queue the queue to push an item into
 * @param obj the object to add to the queue
 * @param objsize the size of the object to add
 * @param flag the flag for the object
 * @return 0 on success, non-zero on error
 *
 * @brief pushes an item onto the end of a queue
 */
 int8_t push_QueueList(QueueList * queue, void *obj, size_t objsize,
                         int flag);
#define convert_QueueList(A,B,C,D)	push_QueueList((A),(B),(C),(D))

create_iter_prototypes(QueueList)

/* beginning of vector based deque functions */
#ifndef QUEUEVECTOR_DEFINED
#define QUEUEVECTOR_DEFINED
typedef struct _array_struct QueueVector;
#endif

#ifndef QUEUEVECTOR_ITER_DEFINED
#define QUEUEVECTOR_ITER_DEFINED
typedef struct _array_iter ITER(QueueVector);
#endif

COMMON_FUNCTION_PROTOTYPES(QueueVector)

int8_t pop_QueueVector(QueueVector *queue);
void * front_QueueVector(QueueVector *queue);
int8_t push_QueueVector(QueueVector *queue, void *obj, size_t objsize, int flag);
int8_t resize_QueueVector(QueueVector *queue,size_t size);

#define convert_QueueVector(A,B,C,D)	push_QueueVector((A),(B),(C),(D))

create_iter_prototypes(QueueVector)

#endif
