
/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @author Micah Villmow
 * @brief a reusable deque class that takes any object
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "node.h"
#include "deque.h"
#include "gen/error_macros.h"
#include "gen/access_macros.h"
#include "gen/control_macros.h"
#include "gen/function_macros.h"
#include "gen/control_arr_macros.h"
#include "gen/function_signatures.h"

#ifdef NUM_LINKS
#undef NUM_LINKS
#endif
#define NUM_LINKS 2
      
F_CONSTRUCT(DequeList) {
  CHECK_VARN(obj, EINVAL);
  if (S(obj)) {
    destruct(DequeList, obj);
  }
  PTR_STRUCT_SETUP(obj,datasize,flag);
  API_DEFAULT_SETUP(obj);
  return 0;
}

F_CONSTRUCT_FUNC(DequeList) {
  PTR_STRUCT_SETUP(obj,datasize,flag);
  obj->API.alloc = alloc;
  obj->API.dealloc = dealloc;
  obj->API.cmp = cmp;
  obj->API.print = print;
  obj->API.copy = copy;
  return 0;
}

F_DESTRUCT(DequeList) {
  Node *tmp, *ptr;
  CHECK_VARN(obj, EINVAL);
  clear(DequeList, obj);
  for (ptr = FL(obj); ptr; ptr = tmp) {
    tmp = N(ptr);
    destruct(Node, ptr);
  }
  memset(obj,0,sizeof *obj);
  return 0;
}

int8_t convert_DequeList(DequeList *deque, void *obj, size_t objsize, int flag) {
	return push_back_DequeList(deque,obj,objsize,flag);
}
 int8_t
push_back_DequeList(DequeList * deque, void *obj, size_t objsize, int flag)
{
  Node *tmp;

  CHECK_VARN(deque, EINVAL);
  CHECK_VARN(obj, EINVAL);
  CHECK_OBJSIZE(deque,objsize,EINVAL);

  INITIALIZE_NODE(tmp,deque,obj,flag);

  if (!S(deque)) {
    H(deque) = T(deque) = tmp;
    S(deque)++;
  } else {
    ADD_BACK(tmp, deque);
  }
  return SUCCESS;
allocobjfail:
  ADD_FREE_NODE(deque,tmp);
allocfail:
  return EALLOCF;
}

int8_t
push_front_DequeList(DequeList *deque, void *obj, size_t objsize, int flag) {
  Node *tmp;

  CHECK_VARN(deque, EINVAL);
  CHECK_VARN(obj, EINVAL);
  CHECK_OBJSIZE(deque,objsize,EINVAL);

  INITIALIZE_NODE(tmp,deque,obj,flag);

  if (!S(deque)) {
    H(deque) = T(deque) = tmp;
    S(deque)++;
  } else {
    ADD_FRONT(tmp, deque);
  }

  return SUCCESS;
allocobjfail:
  ADD_FREE_NODE(deque,tmp);
allocfail:
  return -EALLOCF;
}

 int8_t
pop_front_DequeList(DequeList * deque)
{
  Node *tmp;

  CHECK_VARN(deque, EINVAL);

  if (!H(deque) || !S(deque)) {
    ERRORMSG(cannot pop front);
    return -1;
  }
  tmp = H(deque);
  REMOVE_FRONT(deque);
  DELETE_OBJPTR(deque,tmp);
  ADD_FREE_NODE(deque, tmp);
  return SUCCESS;
}

int8_t 
pop_back_DequeList(DequeList *deque) {
	Node *tmp;
	CHECK_VARN(deque,EINVAL);

	if(!T(deque) || !S(deque)) {
		ERRORMSG(cannot pop back);
		return -1;
	}
	tmp = T(deque);
	REMOVE_BACK(deque);
	DELETE_OBJPTR(deque,tmp);
	ADD_FREE_NODE(deque,tmp);
	return SUCCESS;
}

 void *
front_DequeList(DequeList * deque)
{
  CHECK_VARN(deque, NULL);
  CHECK_VARE(H(deque), NULL);
  return H(deque)->objptr;
}

void *
back_DequeList(DequeList *deque) {
	CHECK_VARN(deque,NULL);
	CHECK_VARE(T(deque),NULL);
	return T(deque)->objptr;
}
F_CLEAR(DequeList) {
  PTR_CLEAR(DequeList,obj);
  return 0;
}

F_EMPTY(DequeList) {
	CHECK_VARN(obj, EEMPTY);
	CHECK_VARN(H(obj), EEMPTY);
	return !S(obj);
}

F_SIZE(DequeList) {
	CHECK_VARN(obj,0);
	return S(obj);
}

create_iter_func(Ptr_Based,DequeList)

function(size_of, DequeList)
function(set_compare, DequeList)
function(set_print, DequeList)
function(set_alloc, DequeList)
function(set_dealloc, DequeList)
function(set_copy, DequeList)
function(duplicate_ptr_struct,DequeList)

F_CONSTRUCT(DequeVector) {
	CHECK_VARN(obj,EINVAL);
	ARR_CONSTRUCT(DequeVector,obj,datasize,flag);
	return 0;
}


F_CONSTRUCT_FUNC(DequeVector) {
	CHECK_VARN(obj,EINVAL);
	ARR_STRUCT_SETUP(obj,datasize,flag);
	obj->API.alloc = alloc;
	obj->API.dealloc = dealloc;
	obj->API.cmp = cmp;
	obj->API.print = print;
	obj->API.copy = copy;
	return 0;
}

F_DESTRUCT(DequeVector) {
	CHECK_VARN(obj,EINVAL);
	clear(DequeVector,obj);
	free(M(obj));
	memset(obj,0,sizeof *obj);
	return 0;
}

int8_t pop_front_DequeVector(DequeVector *deque) {
	CHECK_VARN(deque,EINVAL);
	CHECK_VARN(H(deque),EINVAL);
	if(!S(deque)) {
		return 0;
	}

	ARR_POP_FRONT(DequeVector,deque);

	return 0;
}
int8_t pop_back_DequeVector(DequeVector *deque) {
	CHECK_VARN(deque,EINVAL);
	CHECK_VARN(T(deque),EINVAL);
	if(!S(deque)) {
		return 0;
	}

	ARR_POP_BACK(DequeVector,deque);

	return 0;
}

void *back_DequeVector(DequeVector *deque) {
	CHECK_VARN(deque,NULL);
	CHECK_VARN(T(deque),NULL);
	return T(deque);
}
void *front_DequeVector(DequeVector *deque) {
	CHECK_VARN(deque,NULL);
	CHECK_VARN(H(deque),NULL);
	return H(deque);
}

F_CLEAR(DequeVector) {
	CHECK_VARN(obj,EINVAL);
	if(!C(obj)) {
		return 0;
	}
	ARR_CLEAR(DequeVector,obj);
	return 0;
}

int8_t push_front_DequeVector(DequeVector *deque, void *obj, size_t objsize, int flag) {
	CHECK_VARN(deque,EINVAL);
	CHECK_VARN(obj,EINVAL);
	if(objsize != O(deque)) {
		flag = 0;
		return EINVAL;
	}

	ARR_PUSH_FRONT(DequeVector,deque,obj,objsize);

	return 0;
}

int8_t push_back_DequeVector(DequeVector *deque, void *obj, size_t objsize, int flag) {
	CHECK_VARN(deque,EINVAL);
	CHECK_VARN(obj,EINVAL);
	if(objsize != O(deque)) {
		flag = 0;
		return EINVAL;
	}

	ARR_PUSH_BACK(DequeVector,deque,obj,objsize);

	return 0;
}

int8_t resize_DequeVector(DequeVector *deque,size_t size) {
	void *ptr = NULL;
	CHECK_VARN(deque,EINVAL);
	CHECK_VARA(ptr = malloc(size * O(deque)),EALLOCF);
	
	ARR_COPY_WRAP(DequeVector,ptr,deque,size);

	ARR_SETUP_POINTERS(DequeVector,ptr,deque,size);
	return 0;
}
create_iter_func(Arr_Based,DequeVector)

function(size_of, DequeVector)
function(set_compare, DequeVector)
function(set_print, DequeVector)
function(set_alloc, DequeVector)
function(set_dealloc, DequeVector)
function(set_copy, DequeVector)
function(duplicate_arr_struct,DequeVector)
