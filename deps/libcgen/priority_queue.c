/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file priority_queue.c
 * @brief Generic Priority Queue Implementation
 *
 * Implementaiton of a generic priority queue
 */
#include "priority_queue.h"
#include "heap.h"
#include "gen/gen_macros.h"
#include "gen/error_macros.h"
#include "gen/function_macros.h"
#include "gen/control_arr_macros.h"
#include "gen/function_signatures.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

PrioNode* find_data_PrioQueue(PrioQueue* obj, void* data, size_t datasize);

int priocmp(const void *a, const void *b,size_t size);
F_CONSTRUCT(PrioQueue) {
	int res;
	CHECK_VARN(obj,EINVAL);
	CHECK_VARA(obj->data = malloc(sizeof *obj->data),EALLOCF);
	res = construct(Heap,obj->data,datasize,flag);
	if(!res) {
		obj->data->API.cmp = priocmp;
	}
	return res;
}

F_CONSTRUCT_FUNC(PrioQueue) {
	CHECK_VARN(obj,EINVAL);
	CHECK_VARA(obj->data = malloc(sizeof *obj->data),EALLOCF);
	return construct_func(Heap,obj->data,datasize,flag,alloc,dealloc,priocmp,print,copy);
}

F_DESTRUCT(PrioQueue) {
	if(obj) {
		destruct(Heap,obj->data);
		free(obj->data);
		return SUCCESS;
	}
	return EINVAL;
}

F_CLEAR(PrioQueue) {
	CHECK_VARN(obj,EINVAL);
	return clear(Heap,obj->data);
}


F_PRINT(PrioQueue) {
	CHECK_VARN(obj,EINVAL);
	return print_all(Heap,obj->data);
}

F_EMPTY(PrioQueue) {
	CHECK_VARN(obj, EEMPTY);
	return empty(Heap,obj->data);
}


F_SIZE(PrioQueue) {
	CHECK_VARN(obj,0);
	return size(Heap,obj->data);
}

F_DUMP(PrioQueue) {
	CHECK_VARN(obj,EINVAL);
	return dump(Heap,obj->data);
}

F_SET_COMPARE(PrioQueue) {
	CHECK_VARN(obj,EINVAL);
	return set_compare(Heap,obj->data,cmp);
}

F_SET_PRINT(PrioQueue) {
	CHECK_VARN(obj,EINVAL);
	return set_print(Heap,obj->data,print);
}

F_SET_ALLOC(PrioQueue) {
	CHECK_VARN(obj,EINVAL);
	return set_alloc(Heap,obj->data,alloc);
}

F_SET_DEALLOC(PrioQueue) {
	CHECK_VARN(obj,EINVAL);
	return set_dealloc(Heap,obj->data,dealloc);
}

F_SET_COPY(PrioQueue) {
	CHECK_VARN(obj,EINVAL);
	return set_copy(Heap,obj->data,copy);
}

F_SIZE_OF(PrioQueue) {
	CHECK_VARN(obj,0);
	return size_of(Heap,obj->data);
}


int8_t insert_PrioQueue(PrioQueue *obj, int32_t priority, void *data, size_t datasize) {
	PrioNode *ptr;
	CHECK_VARN(obj,EINVAL);
	CHECK_VARN(data,EINVAL);
	CHECK_VARA(ptr = malloc(sizeof *ptr),EALLOCF);
	if(!(ptr->data = obj->data->API.alloc(datasize))) {
		free(ptr);
		return EALLOCF;
	}
	ptr->priority = priority;
	obj->data->API.copy(ptr->data,data,datasize);
	return insert(Heap,(obj->data),ptr,STATIC);
}

F_DUPLICATE(PrioQueue) {
	PrioQueue *dst;
	CHECK_VARN(obj,NULL);
	CHECK_VARA(dst = malloc(sizeof *dst),NULL);
	if(!(dst->data = duplicate(Heap,obj->data))) {
		free(dst);
		return NULL;
	} else {
		return dst;
	}
}

void *top_data_PrioQueue(PrioQueue *obj) {
	CHECK_VARN(obj,NULL);
	if(empty(Heap,(obj->data))) {
		return NULL;
	}
	return ((PrioNode *)top(Heap,(obj->data)))->data;
}

int32_t top_prio_PrioQueue(PrioQueue* obj) {
	CHECK_VARN(obj,INT_MIN);
	if(empty(Heap,(obj->data))) {
		return INT_MIN;
	}
	return ((PrioNode *)top(Heap,(obj->data)))->priority;
}

int8_t pop_PrioQueue(PrioQueue *obj) {
	CHECK_VARN(obj,EINVAL);
	return pop(Heap,(obj->data));
}

int8_t inc_prio_PrioQueue(PrioQueue *obj, int32_t amt, void *data, size_t datasize) {
	PrioNode *ptr;
	CHECK_VARN(obj,EINVAL);
	CHECK_VARN(data,EINVAL);
	CHECK_OBJSIZE((obj->data),datasize,EINVAL);
	if(empty(Heap,(obj->data))) {
		return EINVAL;
	}
	if(!(ptr = find_data_PrioQueue(obj,data,datasize))) {
		return ENOVAL;
	}
	ptr->priority += amt;
	return SUCCESS;
}

int8_t dec_prio_PrioQueue(PrioQueue *obj, int32_t amt, void *data, size_t datasize) {
	PrioNode *ptr;
	CHECK_VARN(obj,EINVAL);
	CHECK_VARN(data,EINVAL);
	CHECK_OBJSIZE((obj->data),datasize,EINVAL);
	if(empty(Heap,(obj->data))) {
		return EEMPTY;
	}
	if(!(ptr = find_data_PrioQueue(obj,data,datasize))) {
		return ENOVAL;
	}
	ptr->priority -= amt;
	return SUCCESS;
}

int32_t find_prio_PrioQueue(PrioQueue *obj, void *data, size_t datasize) {
	PrioNode *ptr;
	CHECK_VARN(obj,INT_MIN);
	CHECK_VARN(data,INT_MIN);
	CHECK_OBJSIZE((obj->data),datasize,INT_MIN);
	if(empty(Heap,(obj->data))) {
		return EEMPTY;
	}
	if(!(ptr = find_data_PrioQueue(obj,data,datasize))) {
		return ENOVAL;
	}
	return ptr->priority;
}

int8_t resize_PrioQueue(PrioQueue *obj, size_t size) {
	CHECK_VARN(obj,EINVAL);
	resize(Heap,(obj->data),size);
	return SUCCESS;
}

PrioNode* find_data_PrioQueue(PrioQueue* obj, void* data, size_t datasize) {
	Heap *heap;
	PrioNode *ptr;
	CHECK_VARN(obj,NULL);
	CHECK_VARN(data,NULL);
	CHECK_VARN(obj->data,NULL);
	CHECK_OBJSIZE((obj->data),datasize,NULL);
	if(empty(Heap,obj->data)) {
		return NULL;
	}
	for(ptr = (PrioNode *)heap->mem;
		ptr < &((PrioNode*)heap->mem)[heap->capacity];
		ptr++) {
		if(memcmp(ptr->data,data,datasize) == 0) {
			return ptr;
		}
	}
	return NULL;
}

int priocmp(const void *a, const void *b,size_t UNUSED size) {
	return (*(const uint32_t *)a) - (*(const uint32_t *)b);
}
/*function(size_of, PrioQueue)
function(set_compare, PrioQueue)
function(set_print, PrioQueue)
function(set_copy, PrioQueue)
function(set_alloc, PrioQueue)
function(set_dealloc, PrioQueue)*/
