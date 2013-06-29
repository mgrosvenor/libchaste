
/**
 * Copyright 2006, Micah Villmow, All Rights Reserved
 * @file vector.c
 * @author Micah Villmow
 * @brief a reusable vector class that takes any object
 *
 * The base class that other data structures are created from
 */
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "gen/data_types.h"
#include "vector.h"
#include "gen/error_macros.h"
#include "gen/access_macros.h"
#include "gen/function_macros.h"
#include "gen/control_arr_macros.h"
#include "gen/function_signatures.h"

#define D(X)	((char *)(H((X))))

F_CONSTRUCT(Vector) {
  CHECK_VARN(obj, EINVAL);
  CHECK_VARE(datasize, EINVAL);
  CHECK_VARE(flag, EINVAL);
  ARR_CONSTRUCT(Vector,obj,datasize,flag);
  return SUCCESS;
}

F_CONSTRUCT_FUNC(Vector) {
  CHECK_VARN(obj,EINVAL);
  CHECK_VARE(datasize, EINVAL);
  CHECK_VARE(flag, EINVAL);
  ARR_STRUCT_SETUP(obj,datasize,flag);
  obj->API.alloc = alloc;
  obj->API.dealloc = dealloc;
  obj->API.cmp = cmp;
  obj->API.print = print;
  obj->API.copy = copy;
  return SUCCESS;
}

F_DESTRUCT(Vector) {
  CHECK_VARN(obj, EINVAL);
  clear(Vector,obj);
  free(M(obj));
  memset(obj,0,sizeof *obj);
  return SUCCESS;
}

F_CLEAR(Vector) {
	CHECK_VARN(obj,EINVAL);
	if(!C(obj)) {
		return SUCCESS;
	}
	ARR_CLEAR(Vector,obj);
	return SUCCESS;
}

/*Vector* duplicate_Vector(Vector* src) {
	Vector *dst;
	size_t off;
	CHECK_VARN(src,NULL);
	CHECK_VARA(dst = malloc(sizeof *dst),NULL);
	dst->size = src->size;
	dst->capacity = src->capacity;
	dst->objsize = src->objsize;
	dst->objfree = FREEOBJ;
	dst->API.alloc = src->API.alloc;
	dst->API.dealloc = src->API.dealloc;
	dst->API.copy = src->API.copy;
	dst->API.cmp = src->API.cmp;
	dst->API.rcmp = src->API.rcmp;
	dst->API.print = src->API.print;
	dst->end = dst->head = dst->mem = dst->tail = NULL;
	if(!(M(dst) = malloc(src->capacity * O(src)))) {
		free(dst);
		return NULL;
	}
	off = S(src) * O(src);
	printf("Duplicating vector of size %d and capacity %d offset = %x\n",dst->size,dst->capacity,off);
	printf("src = %p, %p, %p, %p\n",M(src),H(src),T(src),src->end);
	memcpy(M(dst),M(src),off);
	if(M(src) == H(src)) {
		printf("head is pointing to the front\n");
		H(dst) = M(dst);
	} else {
		ptrdiff_t offset = (char *)H(src) - (char *)M(src);
		printf("head offset is %d\n",offset / O(src));
		H(dst) = (char *)M(dst) + offset;
	}
	if(T(src) == M(src)) {
		T(dst) = M(dst);
		printf("tail is pointing to the front\n");
	} else {
		ptrdiff_t offset = (char *)T(src) - (char *)M(src);
		printf("tail offset is %d\n",offset/O(src));
		T(dst) = (char *)M(dst) + offset;
	}
	dst->end = (char *)M(dst) + (O(dst) * C(dst));
	return dst;
}*/

int8_t
insert_at_Vector(Vector * vec, void *obj, uint32_t loc)
{
  size_t offset;
  CHECK_VARN(vec, EINVAL);
  CHECK_VARN(obj, EINVAL);
  if (loc > C(vec)) {
	  return EOOB;
  }
  offset = O(vec) * loc;
  if(!S(vec)) {
	  if(M(vec) == NULL) {
		  CHECK_VARA(M(vec) = malloc((offset << 1) == 0 ? (O(vec)):(offset << 1)),EALLOCF);
		  C(vec) = (offset << 1) == 0 ? (1):(offset << 1);
	  }
	  H(vec) = M(vec);
	  T(vec) = ((char *)H(vec) + O(vec));
	  vec->end = ((char *)(M(vec))) + (C(vec) * O(vec));
	  offset = 0;
	  S(vec) = 1;
  }
  if(H(vec) < T(vec)) {
	  /* No wraparound */
	  if (loc >= S(vec)) {
	    S(vec) = loc + 1;
	    T(vec) = offset + D(vec);
	  }
  } else if((void *)(D(vec) + offset) < vec->end) {
	  /* Wraparound, but index is before end */
	  if (loc >= S(vec)) {
		  S(vec) = loc + 1;
		  T(vec) = D(vec) + offset;
	  }
  } else {
	  /* Wraparound, with index at beginning */
	  ptrdiff_t spaces = ((char *)vec->end - D(vec));
	  if(loc > S(vec)) {
		  S(vec) = loc;
		  T(vec) = (offset - spaces) + D(vec);
	  }
	  offset -= spaces;
  }
  vec->API.copy(D(vec) + offset, obj, O(vec));
  return SUCCESS;
}

int8_t push_back_Vector(Vector *vec, void *obj, size_t objsize, int flag) {
	CHECK_VARN(vec,EINVAL);
	CHECK_VARN(obj,EINVAL);
	if(objsize != O(vec)) {
		flag = 0;
		return EINVAL;
	}
	ARR_PUSH_BACK(Vector,vec,obj,objsize);
	return SUCCESS;
}

int8_t push_front_Vector(Vector *vec, void *obj, size_t objsize, int flag) {
	CHECK_VARN(vec,EINVAL);
	CHECK_VARN(obj,EINVAL);
	if(objsize != O(vec)) {
		flag = 0;
		return EINVAL;
	}

	ARR_PUSH_FRONT(Vector,vec,obj,objsize);

	return SUCCESS;
}

int8_t pop_back_Vector(Vector *vec) {
	CHECK_VARN(vec,EINVAL);
	CHECK_VARN(T(vec),EINVAL);
	if(!S(vec)) {
		return EEMPTY;
	}

	ARR_POP_BACK(Vector,vec);
	return SUCCESS;
}

int8_t pop_front_Vector(Vector *vec) {
	CHECK_VARN(vec,EINVAL);
	CHECK_VARN(H(vec),EINVAL);
	if(!S(vec)) {
		return EEMPTY;
	}

	ARR_POP_FRONT(Vector,vec);

	return SUCCESS;
}

void *front_Vector(Vector *vec) {
	CHECK_VARN(vec,NULL);
	CHECK_VARN(H(vec),NULL);
	return H(vec);
}

void *back_Vector(Vector *vec) {
	CHECK_VARN(vec,NULL);
	CHECK_VARN(T(vec),NULL);
	return T(vec);
}

void *
return_at_Vector(Vector * vec, uint32_t loc)
{
  register uint32_t offset = (loc * O(vec));
  CHECK_VARN(vec, NULL);
  if(loc >= S(vec)) {
	  return NULL;
  }
  if(H(vec) < T(vec)) {
	  /* No wrap around */
	  return (void *)(D(vec) + offset);
  } else if((void *)((D(vec) + offset)) < vec->end) {
		  /* wrap around exists, but the position
		   * is before the end of the array */
		  return (void *)(D(vec) + offset);
  } else {
	  /* wrap around exists and position is 
	   * at beginning of array */
	  ptrdiff_t spaces = ((char *)vec->end - D(vec));
	  return (void *)((char *)vec->mem + (offset - spaces));
  }
}

int8_t
resize_Vector(Vector * vec, size_t size)
{
  void *ptr;
  size_t offset = size * O(vec);

  CHECK_VARN(vec, EINVAL);
  CHECK_VARA(ptr = malloc(offset),EALLOCF);
  ARR_COPY_WRAP(Vector,ptr,vec,size);
  /*offset = S(vec) * O(vec);
  if(M(vec)) {
	  if(S(vec) == 0) {
		  printf("%d\n",__LINE__);
	  } else if(H(vec) < T(vec)) {
		  printf("%d\n",__LINE__);
		  memcpy(ptr,H(vec),((char *)T(vec) - (char *)H(vec)));
	  } else if((void *)((char *)(H(vec)) + offset) < vec->end) {
		  printf("%d\n",__LINE__);
		  memcpy(ptr,H(vec),offset);
	  } else {
		  ptrdiff_t spaces = ((char *)vec->end - (char *)H(vec));
		  memcpy(ptr,H(vec),(size_t)spaces);
		  memcpy((char *)(ptr + spaces),M(vec),O(vec) * (S(vec) - spaces/O(vec)));
	  }
	  free(M(vec));
  }*/
  ARR_SETUP_POINTERS(Vector,ptr,vec,size);
  return SUCCESS;
}

create_iter_func(Arr_Based,Vector)

function(size_of, Vector)
function(set_compare, Vector)
function(set_print, Vector)
function(set_copy, Vector)
function(set_alloc, Vector)
function(set_dealloc, Vector)
/*function(set_free_objects, Vector)
function(set_arr_object_size,Vector)*/
function(duplicate_arr_struct,Vector)
