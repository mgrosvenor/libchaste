/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file heap.c
 * @brief Implementation of various generic heap functions
 */

#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "heap.h"
#include "gen/gen_macros.h"
#include "gen/data_types.h"
#include "gen/error_macros.h"
#include "gen/function_macros.h"
#include "gen/control_arr_macros.h"
#include "gen/function_signatures.h"


F_CONSTRUCT(Heap) {
	CHECK_VARN(obj,EINVAL);
	ARR_CONSTRUCT(Heap,obj,datasize,flag);
	resize(Heap,obj,20);
	return SUCCESS;
}


F_CONSTRUCT_FUNC(Heap) {
	CHECK_VARN(obj,EINVAL);
	ARR_STRUCT_SETUP(obj,datasize,flag);
	obj->API.alloc = alloc;
	obj->API.dealloc = dealloc;
	obj->API.cmp = cmp;
	obj->API.print = print;
	obj->API.copy = copy;
	return SUCCESS;
}

F_DESTRUCT(Heap) {
	CHECK_VARN(obj,EINVAL);
	clear(Heap,obj);
	free(M(obj));
	memset(obj,0,sizeof *obj);
	return SUCCESS;
}

F_CLEAR(Heap) {
	CHECK_VARN(obj,EINVAL);
	if(!C(obj)) {
		return SUCCESS;
	}
	ARR_CLEAR(Heap,obj);
	return SUCCESS;
}

F_PRINT(Heap) {
	size_t base = 1, x = 0;
	CHECK_VARN(obj,EINVAL);
	if(T(obj) == H(obj)) {
		return SUCCESS;
	}
	x = S(obj);
	while(x > 0) {
		x = x >> 1;
		base = base << 1;
	}
	if(base > C(obj)) {
		base = C(obj);
	}	
	for(x = 0; x < S(obj); x++) {
		obj->API.print((char *)H(obj) + (x* O(obj)));
	}
	printf("\n");
	return SUCCESS;
}

F_DUMP(Heap) {
	CHECK_VARN(obj,EINVAL);
	printf("Size: %u Capacity: %u Objsize: %u Objfree: %d\n",(unsigned int)S(obj),(unsigned int)C(obj),(unsigned int)obj->objsize,obj->objfree);
	printf("Mem: %p Head: %p Tail: %p End: %p\n",M(obj),H(obj),T(obj),obj->end);
	return SUCCESS;
}

F_EMPTY(Heap) {
	CHECK_VARN(obj, EINVAL);
	CHECK_VARN(obj->mem, EINVAL);
	return H(obj) != T(obj);
}

F_SIZE(Heap) {
	CHECK_VARN(obj,0);
	CHECK_VARN(obj->mem,0);
	return ((char *)T(obj) - (char *)H(obj)) / O(obj);
}

int8_t insert_Heap(Heap *obj, void *data, size_t datasize, int flag) {
	unsigned int curind = 0;
	CHECK_VARN(obj,EINVAL);
	CHECK_VARN(data,EINVAL);
	CHECK_VARN(T(obj),EINVAL);
	CHECK_OBJSIZE(obj,datasize,EINVAL);
	if((S(obj)) == (C(obj)-1)) {
		resize(Heap,obj,C(obj)<<1);
	}
	curind = S(obj);
	if(S(obj) > 0) {
		while(obj->API.cmp((char *)H(obj) + (((curind-1) >> 1)*O(obj)),data,datasize) > 0) {
			obj->API.copy((char *)H(obj) + ((curind) * O(obj)),(char *)H(obj) + ((((curind-1)>>1)) * O(obj)), datasize);
			curind = (curind - 1) >> 1;
			if(curind == 0) {
				break;
			}
		}
	}
	S(obj)++;
	obj->API.copy((char *)H(obj) + ((curind) * O(obj)),data,datasize);
	T(obj) = (char *)T(obj) + O(obj);
	return SUCCESS;
}

void* top_Heap(Heap *obj) {
	CHECK_VARN(obj,NULL);
	if(H(obj) == T(obj)) {
		return NULL;
	}
	return H(obj);
}

int8_t pop_Heap(Heap* obj) {
	void *minptr, *lastptr;
	CHECK_VARN(obj,EINVAL);
	if(S(obj) == 0) {
		return ENOVAL;
	}
	S(obj)--;
	if(S(obj) > 0) {
		unsigned int i, child = 1;
		i = 0;
		minptr = H(obj);
		lastptr = (char *)T(obj) - O(obj);
		child = 1;
		while(child < ((S(obj)-1))) {
			if(child < (S(obj)) && obj->API.cmp((char *)H(obj) + (child * O(obj)),(char *)H(obj) + ((child + 1) * O(obj)),O(obj)) > 0) {
				child++;
			}
			if(obj->API.cmp(lastptr,(char *)H(obj) + ((child) * O(obj)),O(obj)) > 0) {
				obj->API.copy((char *)H(obj) + (i * O(obj)),(char *)H(obj) + ((child) * O(obj)),O(obj));
			} else {
				break;
			}
			i = child;	
			child = (i << 1) + 1;
		}
		obj->API.copy((char *)H(obj) + (i*O(obj)),lastptr,O(obj));
	}
	T(obj) = (char *)T(obj) - O(obj);
	return SUCCESS;
}

int8_t resize_Heap(Heap *obj, size_t size) {
	void *ptr;
	size_t offset = size * O(obj);
	CHECK_VARN(obj,EINVAL);
	CHECK_VARA(ptr = malloc(offset),EALLOCF);
	memset(ptr,0,offset);
	ARR_COPY_WRAP(Heap,ptr,obj,size);

	ARR_SETUP_POINTERS(Heap,ptr,obj,size);

	return SUCCESS;
}

int8_t merge_Heap(Heap *obj, Heap *src) {
	char *ptr;
	CHECK_VARN(obj,EINVAL);
	CHECK_VARN(src,EINVAL);
	for(ptr = H(src); ptr != T(src); ptr = ptr + O(src)) {
		insert_Heap(obj,ptr,O(src),STATIC);
	}
	return SUCCESS;
}

function(duplicate_arr_struct,Heap)
function(size_of,Heap)
function(set_compare,Heap)
function(set_print,Heap)
function(set_copy,Heap)
function(set_alloc,Heap)
function(set_dealloc,Heap)
