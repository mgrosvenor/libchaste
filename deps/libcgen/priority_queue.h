/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file priority_queue.h
 * @brief Generic Priority Queue implementation
 * 
 * The generic priority queue implementations, when completed
 * will be able to be based on any heap type.
 */
#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_
#include "gen/data_types.h"
#include "gen/gen_macros.h"
#include "gen/function_signatures.h"


typedef struct _prio_queue_node {
	/** Priority value for given data */
	int32_t priority;
	/** Pointer to the data */
	void *data;
	/** Size of the data being stored */
	size_t datasize;
} PrioNode;

typedef struct _prio_queue {
	/** The heap used to store the data */
	Heap *data;
} PrioQueue;

COMMON_FUNCTION_PROTOTYPES(PrioQueue)

/**
 * @param obj the object to insert the items in
 * @param priority the priority of the data
 * @param data the data to be inserted
 * @param datasize the size of the data
 * @return 0 on success, non-zero on error
 */
int8_t insert_PrioQueue(PrioQueue *obj,int32_t priority,void* data, size_t datasize);

/**
 * @param obj the object to retrieve the data of
 * @return data of the top item or NULL on error
 * @brief retrieves the data of the top item in the queue
 */
void *top_data_PrioQueue(PrioQueue* obj);
/**
 * @param obj the object to retrieve the top priority of
 * @return priority of top object or INT_MIN on error
 * @brief retrieves the priority of the top item in the queue
 */
int32_t top_prio_PrioQueue(PrioQueue *obj);
/**
 * @param obj the object to pop the top item off of
 * @brief pops the item that is at the top of the queue
 * @return 0 on success, non-zero on error
 */
int8_t pop_PrioQueue(PrioQueue *obj);
/**
 * @param obj the object that contains the data
 * @param amt the amount to increment the priority by
 * @param data the data to increase the priority of
 * @param datasize the size of the data
 * @brief increment the priority of the given data
 * @return 0 on success, non-zero on error
 */
int8_t inc_prio_PrioQueue(PrioQueue *obj,int32_t amt,void *data, size_t datasize);
/**
 * @param obj the object that contains the data
 * @param amt the amount to decrement the priority by
 * @param data the data to decrease priority of
 * @param datasize the size of the data
 * @brief decrement the priority of the given data
 * @return 0 on success, non-zero on error
 */
int8_t dec_prio_PrioQueue(PrioQueue *obj,int32_t amt,void *data, size_t datasize);
/**
 * @fn int8_t find_prio(PrioQueue *obj, void *data, size_t datasize)
 * @param obj the object to search
 * @param data the data to look for
 * @param datasize the size of the data
 * @brief Find the corresponding priority of the given data
 * @return priority of data on success, INT_MIN on failure
 */
int32_t find_prio_PrioQueue(PrioQueue *obj, void *data, size_t datasize);
/**
 * @fn int8_t resize_PrioQueue(PrioQueue *obj, size_t size)
 * @param obj the object to resize
 * @param ize the size to resize the object to
 * @return 0 on success, non-zero on error
 * @brief Resizes the Priority Queue
 */

int8_t resize_PrioQueue(PrioQueue *obj,size_t size);

#endif
