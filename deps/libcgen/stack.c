
/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @author Micah Villmow
 * @brief a reusable stack class that takes any object
 *
 * the stack class offers two flavors, a stacklist and
 * a stackvector, depending on which underlying structure
 * the programmer prefers
 */
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "stack.h"
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

int8_t
push_StackList(StackList * stack, void *obj, size_t objsize, int8_t flag)
{
  Node *tmp;

  CHECK_VARN(stack, EINVAL);
  CHECK_VARN(obj, EINVAL);
  CHECK_OBJSIZE(stack,objsize,EINVAL);

  INITIALIZE_NODE(tmp,stack,obj,flag);

  if (!S(stack)) {
    H(stack) = T(stack) = tmp;
    S(stack)++;
  } else {
    ADD_BACK(tmp, stack);
  }
  return SUCCESS;
allocobjfail:
  ADD_FREE_NODE(stack,tmp);
allocfail:
  return EALLOCF;
}

int8_t
pop_StackList(StackList * stack)
{
  Node *tmp;

  CHECK_VARN(stack, EINVAL);

  if (!T(stack) || !S(stack)) {
    ERRORMSG(cannot pop top);
    return -1;
  }
  tmp = T(stack);
  REMOVE_BACK(stack);
  DELETE_OBJPTR(stack,tmp);
  ADD_FREE_NODE(stack, tmp);
  return SUCCESS;
}

void *
top_StackList(StackList * stack)
{
	CHECK_VARN(stack,NULL);
	CHECK_VARN(T(stack),NULL);
	return T(stack)->objptr;
}

F_CONSTRUCT(StackList) {
  CHECK_VARN(obj, EINVAL);
  if (S(obj)) {
    destruct(StackList, obj);
  }
  PTR_STRUCT_SETUP(obj,datasize,flag);
  API_DEFAULT_SETUP(obj);
  return 0;
}

F_CONSTRUCT_FUNC(StackList) {
  PTR_STRUCT_SETUP(obj,datasize,flag);
  obj->API.alloc = alloc;
  obj->API.dealloc = dealloc;
  obj->API.cmp = cmp;
  obj->API.print = print;
  obj->API.copy = copy;
  return 0;
}

F_DESTRUCT(StackList) {
  Node *tmp, *ptr;
  CHECK_VARN(obj, EINVAL);
  clear(StackList, obj);
  for (ptr = FL(obj); ptr; ptr = tmp) {
    tmp = N(ptr);
    destruct(Node, ptr);
  }
  memset(obj,0,sizeof *obj);
  return 0;
}

F_CLEAR(StackList) {
  PTR_CLEAR(StackList,obj);
  return 0;
}

/* Array based functions */

create_iter_func(Ptr_Based,StackList)

F_CONSTRUCT(StackVector) {
	CHECK_VARN(obj,EINVAL);
	ARR_CONSTRUCT(StackVector,obj,datasize,flag);
	return 0;
}

F_CONSTRUCT_FUNC(StackVector) {
	CHECK_VARN(obj,EINVAL);
	ARR_STRUCT_SETUP(obj,datasize,flag);
	obj->API.alloc = alloc;
	obj->API.dealloc = dealloc;
	obj->API.cmp = cmp;
	obj->API.print = print;
	obj->API.copy = copy;
	return 0;
}

F_DESTRUCT(StackVector) {
	CHECK_VARN(obj,EINVAL);
	clear(StackVector,obj);
	free(M(obj));
	memset(obj,0,sizeof *obj);
	return 0;
}

int8_t pop_StackVector(StackVector *stack) {
	CHECK_VARN(stack,EINVAL);
	CHECK_VARN(T(stack),EINVAL);
	if(!S(stack)) {
		return 0;
	}
	ARR_POP_BACK(StackVector,stack);
	return 0;
}

void *top_StackVector(StackVector *stack) {
	CHECK_VARN(stack,NULL);
	CHECK_VARN(T(stack),NULL);
	return T(stack);
}

F_CLEAR(StackVector) {
	CHECK_VARN(obj,EINVAL);
	if(!C(obj)) {
		return 0;
	}
	ARR_CLEAR(StackVector,obj);
	return 0;
}

int8_t push_StackVector(StackVector *stack, void *obj, size_t objsize, int flag) {
	CHECK_VARN(stack,EINVAL);
	CHECK_VARN(obj,EINVAL);
	if(objsize != O(stack)) {
		flag = 0;
		return EINVAL;
	}

	ARR_PUSH_BACK(StackVector,stack,obj,objsize);

	return 0;
}

int8_t resize_StackVector(StackVector *stack,size_t size) {
	void *ptr = NULL;
	CHECK_VARN(stack,EINVAL);
	CHECK_VARA(ptr = malloc(size * O(stack)),EALLOCF);
	
	ARR_COPY_WRAP(StackVector,ptr,stack,size);

	ARR_SETUP_POINTERS(StackVector,ptr,stack,size);

	return 0;
}

function(size_of, StackList)
function(set_compare, StackList)
function(set_print, StackList)
function(set_alloc, StackList)
function(set_dealloc, StackList)
function(set_copy, StackList)
function(duplicate_ptr_struct,StackList)




create_iter_func(Arr_Based,StackVector)

function(size_of, StackVector)
function(set_compare, StackVector)
function(set_print, StackVector)
function(set_alloc, StackVector)
function(set_dealloc, StackVector)
function(set_copy, StackVector)
function(duplicate_arr_struct,StackVector)

