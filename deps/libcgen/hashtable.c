/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file hashtable.c
 * @brief implementation of three generic hash tables
 * and some basic hashing functions
 */
#include "gen/stdint.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "hashtable.h"
#include "gen/data_types.h"
#include "gen/error_macros.h"
#include "gen/control_macros.h"

F_CONSTRUCT(OHTable) {
	CHECK_VARN(obj,EINVAL);
	if(obj->capacity > 0) {
		destruct(OHTable,obj);
	}
	obj->capacity = datasize;
	obj->cur_size = 0;
	obj->num_elem = 0;
	obj->objfree = flag;
	obj->hash = num_hash;
	API_DEFAULT_SETUP(obj);
	obj->nodes = malloc(sizeof *obj->nodes * datasize);
	memset(obj->nodes,0,sizeof *obj->nodes * datasize);
	return SUCCESS;
}
F_CONSTRUCT_FUNC(OHTable) {
	CHECK_VARN(obj,EINVAL);
	construct(OHTable,obj,datasize,flag);
  obj->API.alloc = alloc;
  obj->API.dealloc = dealloc;
  obj->API.cmp = cmp;
  obj->API.print = print;
  obj->API.copy = copy;
  return SUCCESS;
}
F_DESTRUCT(OHTable) {
	HashListNode *ptr, *tmp;
	clear(OHTable,obj);
	for(ptr = obj->free_list;ptr; ptr = tmp) {
		tmp = ptr->next;
		free(ptr);
	}
	obj->capacity = 0;
	obj->cur_size = 0;
	obj->num_elem = 0;
	obj->objfree = 0;
	obj->hash = NULL;
	free(obj->nodes);
	return SUCCESS;
}

F_CLEAR(OHTable) {
	HashListNode *ptr, *iter;
	size_t x = 0;
	CHECK_VARN(obj,EINVAL);
	for(x = 0; x < obj->capacity; x++) {
		if(obj->nodes[x].objptr) {
			ptr = &obj->nodes[x];
			DELETE_OBJPTR(obj,ptr);
			iter = ptr->next;
			while(iter) {
				DELETE_OBJPTR(obj,iter);
				iter->objptr = NULL;
				iter->objsize = 0;
				iter->flags = 0;
				ptr = iter->next;
				iter->next = FL(obj);
				FL(obj) = iter;
				iter = ptr;
			}
		}
	}
	return SUCCESS;
}

int8_t insert_OHTable(OHTable *table, void *element, size_t elesize,int8_t flag) {
	int32_t h_index;
	HashListNode *ptr;
	CHECK_VARN(table,EINVAL);
	CHECK_VARN(element,EINVAL);
	h_index = table->hash(element,elesize) % table->capacity;
	if(table->nodes[h_index].objptr) {
		HashListNode *iter;
		iter = &table->nodes[h_index];
		while(1) {
			if(table->API.cmp(iter->objptr,element,elesize) == 0) {
				return SUCCESS;
			}
			if(iter->next) {
				iter = iter->next;
			} else {
				break;
			}
		}
		if(FL(table)) {
			ptr = table->free_list;
			table->free_list = ptr->next;
		} else if(!(ptr = malloc(sizeof *ptr))) {
			return EALLOCF;
		}
		ptr->next = NULL;
		iter->next = ptr;
		ptr->prev = iter;
	} else {
		ptr = &table->nodes[h_index];
		table->cur_size++;
	}
	ptr->objsize = elesize;
	ptr->flags = flag;
	if(flag == DYNAMIC) {
		ptr->objptr = element;
	} else if((ptr->objptr = table->API.alloc(elesize))) {
		table->API.copy(ptr->objptr,element,elesize);
	} else {
		return EALLOCF;
	}
	table->num_elem++;
	return SUCCESS;
}

int8_t delete_OHTable(OHTable *table, void *element, size_t elesize) {
	int32_t h_index;
	CHECK_VARN(table,EINVAL);
	CHECK_VARN(element,EINVAL);
	h_index = table->hash(element,elesize) % table->capacity;
	if(table->nodes[h_index].objptr) {
		HashListNode *iter,*prev;
		iter = &table->nodes[h_index];
		prev = NULL;
		while(1) {
			if(table->API.cmp(iter->objptr,element,elesize) == 0) {
				DELETE_OBJPTR(table,iter);			
				if(!prev) {
					prev = iter->next;
					if(prev) {
						iter->next = prev->next;
						if(iter->next) {
							iter->next->prev = iter;
						}
						iter->objptr = prev->objptr;
						iter->flags = prev->flags;
						iter->objsize = prev->objsize;
						prev->objptr = NULL;
						prev->flags = 0;
						prev->objsize = 0;
						prev->prev = NULL;
						prev->next = FL(table);
						FL(table) = prev;
					} else {
						iter->objptr = 0;
						iter->flags = 0;
						iter->objsize = 0;
						table->cur_size--;
					}
				} else {
					prev->next = iter->next;
					if(iter->next) {
						iter->next->prev = prev;
					}
					iter->objptr = NULL;
					iter->flags = 0;
					iter->objsize = 0;
					iter->prev = NULL;
					iter->next = FL(table);
					FL(table) = iter;
				}
				table->num_elem--;
				return SUCCESS;
			}
			if(iter->next) {
				prev = iter;
				iter = iter->next;
			} else {
				break;
			}
		}
	}
	return ENOVAL;
}

void *find_OHTable(OHTable *table, void *element, size_t elesize) {
	int32_t h_index;
	CHECK_VARN(table,NULL);
	CHECK_VARN(element,NULL);
	h_index = table->hash(element,elesize) % table->capacity;
	if(table->nodes[h_index].objptr) {
		HashListNode *iter;
		iter = &table->nodes[h_index];
		while(1) {
			if(table->API.cmp(iter->objptr,element,elesize) == 0) {
				return iter->objptr;
			}
			if(iter->next) {
				iter = iter->next;
			} else {
				break;
			}
		}
	}
	return NULL;
}

F_PRINT(OHTable) {
	/*size_t x;
	HashListNode *ptr;
	for(x = 0; x < obj->capacity; x++) {
		ptr = &obj->nodes[x];
		printf("%d) ",x);
		do{
			if(ptr->objptr) {
				obj->API.print(ptr->objptr);
			}
			ptr = ptr->next;
		}while(ptr);
		printf("\n");
	}
	printf("\n");*/
	printf("Open Hash obj: %p Capacity: %u Size: %u\n",(void *)obj,(unsigned int)obj->capacity,(unsigned int)obj->cur_size);
	printf("Flags: %d Element: %u\n",obj->objfree,(unsigned int)obj->num_elem);
	printf("Current nodes:\n");
	return SUCCESS;
}

int8_t set_hash_OHTable(OHTable *table, uint32_t (*hash)(void *,size_t)) {
	CHECK_VARN(table,EINVAL);
	CHECK_VARN(hash,EINVAL);
	table->hash = hash;
	return SUCCESS;
}

F_DUPLICATE(OHTable) {
	OHTable* dst;
	OHTableIter *ohiter;
	CHECK_VARN(obj,NULL);
	CHECK_VARN(dst = malloc(sizeof *dst),NULL);
	memset(dst,0,sizeof *dst);
	dst->objfree = FREEOBJ;
	dst->capacity = obj->capacity;
	dst->API.alloc = obj->API.alloc;
	dst->API.dealloc = obj->API.dealloc;
	dst->API.copy = obj->API.copy;
	dst->API.cmp = obj->API.cmp;
	dst->API.print = obj->API.print;
	dst->hash = obj->hash;
	dst->nodes = malloc(sizeof *dst->nodes * dst->capacity);
	memset(dst->nodes,0,sizeof *dst->nodes * dst->capacity);
	ohiter = create(OHTableIter,obj);
	do{
		insert_OHTable(dst,retrieve(OHTableIter,ohiter),ohiter->ptr->objsize,STATIC);
	}while(!next(OHTableIter,ohiter));
	destroy(OHTableIter,ohiter);
	return dst;
}

int8_t prev_OHTableIter(OHTableIter *iter) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(iter->parent,EINVAL);
	if((iter->ptr == iter->parent->nodes) &&
	   (iter->nptr == iter->ptr)) {
		return EINVAL;
	}
	if((iter->nptr == iter->ptr) || (iter->ptr->objptr == NULL)) {
		do {
			if(iter->ptr == iter->parent->nodes) {
				return EINVAL;
			}
			iter->ptr--;
		}while(iter->ptr->objptr == NULL);
		iter->nptr = iter->ptr;
		while(iter->nptr->next) {
			iter->nptr = iter->nptr->next;
		}
	} else {
		iter->nptr = iter->nptr->prev;
	}
	return SUCCESS;
}

int8_t next_OHTableIter(OHTableIter *iter) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(iter->parent,EINVAL);
	if((iter->ptr == (iter->parent->nodes + (iter->parent->capacity-1))) &&
	   (iter->nptr == NULL)) {
		return EINVAL;
	}
	/*if(iter->nptr->objptr)
	fprintf(stderr,"Cur val = %d,ptr = %p, index = %d, nptr = %p\n",*(int *)iter->nptr->objptr,iter->ptr,
			(iter->ptr - iter->parent->nodes),iter->nptr);*/
	if(iter->nptr->next) {
		iter->nptr = iter->nptr->next;
	} else {
		do {
/*			fprintf(stderr,"%d) Ptr = %p, capacity = %p(%d)\n",iter->ptr - iter->parent->nodes,
					iter->ptr,iter->parent->nodes+ (iter->parent->capacity-1),iter->parent->capacity);*/
			if(iter->ptr == (iter->parent->nodes + (iter->parent->capacity-1))) {
				iter->ptr = iter->nptr;
				while(iter->ptr->prev) {
					iter->ptr = iter->ptr->prev;
				}
				return EINVAL;
			}
			iter->ptr++;
		}while(iter->ptr->objptr == NULL);
		iter->nptr = iter->ptr;
	}
	return SUCCESS;
}

int8_t head_OHTableIter(OHTableIter *iter) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(iter->parent,EINVAL);
	iter->ptr = iter->parent->nodes;
	iter->nptr = iter->ptr;
	if(iter->nptr->objptr == NULL) {
		return next(OHTableIter,iter);
	}
	return SUCCESS;
}

int8_t tail_OHTableIter(OHTableIter *iter) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(iter->parent,EINVAL);
	iter->ptr = (iter->parent->nodes + (iter->parent->capacity-1));
	while(!iter->ptr->objptr) {
		iter->ptr--;
	}
	iter->nptr = iter->ptr;
	while(iter->nptr->next) {
		iter->nptr = iter->nptr->next;
	}
	return SUCCESS;
}

int8_t assign_OHTableIter(OHTableIter *iter,OHTable *obj) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(obj,EINVAL);
	iter->ptr = obj->nodes;
	iter->nptr = obj->nodes;
	iter->parent = obj;
	return SUCCESS;
}

void *retrieve_OHTableIter(OHTableIter *iter) {
	CHECK_VARN(iter,NULL);
	CHECK_VARN(iter->nptr,NULL);
	return iter->nptr->objptr;
}

void destroy_OHTableIter(OHTableIter *iter) {
	if(iter) {
		free(iter);
	}
}

OHTableIter *create_OHTableIter(OHTable *obj) {
	OHTableIter *iter;
	CHECK_VARN(obj,NULL);
	CHECK_VARA(iter = malloc(sizeof *iter),NULL);
	iter->ptr = obj->nodes;
	iter->nptr = obj->nodes;
	iter->parent = obj;
	if(iter->ptr->objptr == NULL) {
		next(OHTableIter,iter);
	}
	return iter;
}

int8_t copy_OHTableIter(OHTableIter *dst, OHTableIter *src) {
	CHECK_VARN(src,EINVAL);
	CHECK_VARN(dst,EINVAL);
	dst->ptr = src->ptr;
	dst->nptr = src->nptr;
	dst->parent = src->parent;
	return SUCCESS;
}

int8_t swap_OHTableIter(OHTableIter *first, OHTableIter* second) {
	OHTableIter tmp;
	CHECK_VARN(first,EINVAL);
	CHECK_VARN(second,EINVAL);
	if(first->parent != second->parent) {
		return EINVAL;
	}
	tmp.ptr = first->ptr;
	tmp.nptr = first->nptr;
	first->ptr = second->ptr;
	first->nptr = second->nptr;
	second->ptr = tmp.ptr;
	second->nptr = tmp.nptr;
	return SUCCESS;
}

function(set_compare,OHTable)
function(set_print,OHTable)
function(set_copy,OHTable)
function(set_alloc,OHTable)
function(set_dealloc,OHTable)

F_CONSTRUCT(CHTable) {
	CHECK_VARN(obj,EINVAL);
	if(obj->capacity > 0) {
		destruct(CHTable,obj);
	}
	obj->capacity = datasize;
	obj->cur_size = 0;
	obj->objfree = flag;
	obj->hash = num_hash;
	obj->prob = linear_probing;
	API_DEFAULT_SETUP(obj);
	if(!(obj->data = malloc(sizeof *obj->data * datasize))) {
		return EALLOCF;
	}
	memset(obj->data,0,sizeof *obj->data * datasize);
	return SUCCESS;
}
F_DESTRUCT(CHTable) {
	if(obj) {
		clear(CHTable,obj);
		obj->capacity = 0;
		obj->cur_size = 0;
		obj->objfree = 0;
		obj->hash = NULL;
		obj->prob = NULL;
		free(obj->data);
		obj->data = NULL;
		return SUCCESS;
	}
	return EINVAL;
}

F_CLEAR(CHTable) {
	HashNode *iter;
	CHECK_VARN(obj,EINVAL);
	for(iter = obj->data; 
		iter < &obj->data[obj->capacity]; iter++) {
		if(iter->objptr) {
			if((iter->flags == STATIC) || 
			   (iter->flags == DYNAMIC && obj->objfree == FREEOBJ)) {
				obj->API.dealloc(iter->objptr);
			}
		}
	}
	return SUCCESS;
}
int8_t insert_CHTable(CHTable *table, void *element, size_t elesize, int8_t flag) {
	uint32_t h_index,start;
	uint16_t x = 0;
	CHECK_VARN(table,EINVAL);
	CHECK_VARN(element,EINVAL);
	start = h_index = table->hash(element,elesize) % table->capacity;
	while(table->data[h_index].objptr) {
		if(table->API.cmp(table->data[h_index].objptr,element,elesize) == 0) {
			return SUCCESS;
		}
		x++;
		h_index = table->prob(h_index) % table->capacity;
		if(h_index == start) {
			return EINVAL;
		}
	}
	if(x > table->max_jumps) {
		table->max_jumps = x;
	}
	table->data[h_index].flags = flag;
	table->data[h_index].objsize = elesize;
	if(flag == DYNAMIC) {
		table->data[h_index].objptr = element;
	} else {
		if((table->data[h_index].objptr = table->API.alloc(elesize))) {
			table->API.copy(table->data[h_index].objptr,element,elesize);
		} else {
			return EALLOCF;
		}
	}
	table->cur_size++;
	return SUCCESS;
}
int8_t delete_CHTable(CHTable *table, void *element, size_t elesize) {
	uint32_t h_index, start;
	uint16_t x;
	CHECK_VARN(table,EINVAL);
	CHECK_VARN(element,EINVAL);
	start = h_index = table->hash(element,elesize) % table->capacity;
	for(x = 0; x <= table->max_jumps; x++) {
		if(table->data[h_index].objptr) {
			if(table->API.cmp(table->data[h_index].objptr,element,elesize) == 0) {
				if((table->data[h_index].flags == STATIC) || 
				   (table->data[h_index].flags == DYNAMIC && table->objfree == FREEOBJ)) {
					table->API.dealloc(table->data[h_index].objptr);
				}
				table->data[h_index].objptr = NULL;
				table->data[h_index].objsize = 0;
				table->data[h_index].flags = 0;
				table->cur_size--;
				return SUCCESS;
			}
		}
		h_index = table->prob(h_index) % table->capacity;
		if(h_index == start) {
			break;
		}
	}
	return ENOVAL;
}
void *find_CHTable(CHTable *table, void* element, size_t elesize) {
	uint32_t h_index, start;
	uint16_t x;
	CHECK_VARN(table,NULL);
	CHECK_VARN(element,NULL);
	start = h_index = table->hash(element,elesize) % table->capacity;
	for(x = 0; x <= table->max_jumps; x++) {
		if(table->data[h_index].objptr) {
			if(table->API.cmp(table->data[h_index].objptr,element,elesize) == 0) {
				return table->data[h_index].objptr;
			} 
		}
		h_index = table->prob(h_index) % table->capacity;
		if(h_index == start) {
			break;
		}
	}
	return NULL;
}

F_DUPLICATE(CHTable) {
	CHTable* dst;
	CHTableIter *ohiter;
	CHECK_VARN(obj,NULL);
	CHECK_VARN(dst = malloc(sizeof *dst),NULL);
	memset(dst,0,sizeof *dst);
	dst->objfree = FREEOBJ;
	dst->capacity = obj->capacity;
	dst->API.alloc = obj->API.alloc;
	dst->API.dealloc = obj->API.dealloc;
	dst->API.copy = obj->API.copy;
	dst->API.cmp = obj->API.cmp;
	dst->API.print = obj->API.print;
	dst->hash = obj->hash;
	dst->prob = obj->prob;
	dst->data = malloc(sizeof *dst->data * dst->capacity);
	memset(dst->data,0,sizeof *dst->data * dst->capacity);
	ohiter = create(CHTableIter,obj);
	do{
		insert_CHTable(dst,retrieve(CHTableIter,ohiter),ohiter->ptr->objsize,STATIC);
	}while(!next(CHTableIter,ohiter));
	destroy(CHTableIter,ohiter);
	return dst;
}

int8_t set_hash_CHTable(CHTable *table, uint32_t (*hash)(void *,size_t)) {
	CHECK_VARN(table,EINVAL);
	CHECK_VARN(hash,EINVAL);
	table->hash = hash;
	return SUCCESS;
}

int8_t set_probe_CHTable(CHTable *table, uint32_t (*prob)(uint32_t)) {
	CHECK_VARN(table,EINVAL);
	CHECK_VARN(prob,EINVAL);
	table->prob = prob;
	return SUCCESS;
}

F_PRINT(CHTable) {
	CHECK_VARN(obj,EINVAL);
	printf("Open Hash obj: %p Capacity: %u Size: %u\n",(void *)obj, (unsigned int)obj->capacity,(unsigned int)obj->cur_size);
	printf("Flags: %d \n",obj->objfree);
	return SUCCESS;
}

int8_t prev_CHTableIter(CHTableIter *iter) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(iter->parent,EINVAL);
	do{
		CHECK_VARE((iter->ptr != iter->parent->data),EINVAL);
		iter->ptr--;
	}while(!iter->ptr->objptr);
	return SUCCESS;
}

int8_t next_CHTableIter(CHTableIter *iter) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(iter->parent,EINVAL);
	do{
		CHECK_VARE((iter->ptr != (iter->parent->data + (iter->parent->capacity-1))),EINVAL);
		iter->ptr++;
	}while(!iter->ptr->objptr);
	return SUCCESS;
}

int8_t head_CHTableIter(CHTableIter *iter) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(iter->parent,EINVAL);
	iter->ptr = iter->parent->data;
	if(iter->ptr->objptr == NULL) {
		return next(CHTableIter,iter);
	}
	return SUCCESS;
}

int8_t tail_CHTableIter(CHTableIter *iter) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(iter->parent,EINVAL);
	iter->ptr = iter->parent->data + (iter->parent->capacity - 1);
	if(iter->ptr->objptr == NULL) {
		return prev(CHTableIter,iter);
	}
	return SUCCESS;
}

int8_t assign_CHTableIter(CHTableIter *iter, CHTable *obj) {
	CHECK_VARN(iter,EINVAL);
	CHECK_VARN(obj,EINVAL);
	iter->ptr = obj->data;
	iter->parent = obj;
	if(iter->ptr->objptr == NULL) {
		next(CHTableIter,iter);
	}
	return SUCCESS;
}

void *retrieve_CHTableIter(CHTableIter *iter) {
	CHECK_VARN(iter,NULL);
	CHECK_VARN(iter->parent,NULL);
	return iter->ptr->objptr;
}

void destroy_CHTableIter(CHTableIter *iter) {
	if(iter) {
		free(iter);
	}
}

CHTableIter *create_CHTableIter(CHTable *obj) {
	CHTableIter *iter;
	CHECK_VARN(obj,NULL);
	CHECK_VARA(iter = malloc(sizeof *iter),NULL);
	iter->ptr = obj->data;
	iter->parent = obj;
	if(iter->ptr->objptr == NULL) {
		next(CHTableIter,iter);
	}
	return iter;
}

int8_t copy_CHTableIter(CHTableIter *dst, CHTableIter *src) {
	CHECK_VARN(dst,EINVAL);
	CHECK_VARN(src,EINVAL);
	dst->ptr = src->ptr;
	dst->parent = src->parent;
	return SUCCESS;
}

int8_t swap_CHTableIter(CHTableIter *first, CHTableIter *second) {
	CHTableIter tmp;
	CHECK_VARN(first,EINVAL);
	CHECK_VARN(second,EINVAL);
	if(first->parent != second->parent) {
		return EINVAL;
	}
	tmp.ptr = first->ptr;
	first->ptr = second->ptr;
	second->ptr = tmp.ptr;
	return SUCCESS;
}

function(set_compare,CHTable)
function(set_print,CHTable)
function(set_copy,CHTable)
function(set_alloc,CHTable)
function(set_dealloc,CHTable)



uint32_t char_hash(void *key, size_t len) {
	uint32_t val = 0;
	while(len-- > 0) {
		val = (val << 5) + tolower(*((char *)key++));
	}
	return val;
}

uint32_t num_hash(void *key, size_t len) {
	uint32_t val =0;
	while(len-- > 0) {
		val = (val << 5) + *(char *)((char *)key++);
	}
	return val;
}

uint32_t linear_probing(uint32_t num) {
	return ++num;
}

uint32_t quadratic_probing(uint32_t num) {
	return num * num;
}
