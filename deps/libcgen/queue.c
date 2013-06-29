
/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @author Micah Villmow
 * @brief a reusable queue class that takes any object
 */

#include <stdio.h> 
#include <string.h>

#include "node.h"
#include "queue.h"
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

      
F_CONSTRUCT(QueueList) {
  CHECK_VARN(obj, EINVAL);
  if (S(obj)) {
    destruct(QueueList, obj);
  }
  PTR_STRUCT_SETUP(obj,datasize,flag);
  API_DEFAULT_SETUP(obj);
  return 0;
}

F_CONSTRUCT_FUNC(QueueList) {
  PTR_STRUCT_SETUP(obj,datasize,flag);
  obj->API.alloc = alloc;
  obj->API.dealloc = dealloc;
  obj->API.cmp = cmp;
  obj->API.print = print;
  obj->API.copy = copy;
  return 0;
}

F_DESTRUCT(QueueList) {
  Node *tmp, *ptr;

  CHECK_VARN(obj, EINVAL);
  clear(QueueList, obj);
  for (ptr = FL(obj); ptr; ptr = tmp) {
    tmp = N(ptr);
    destruct(Node, ptr);
  }
  memset(obj,0,sizeof *obj);
  return 0;
}

 int8_t
push_QueueList(QueueList * queue, void *obj, size_t objsize, int flag)
{
  Node *tmp;

  CHECK_VARN(queue, EINVAL);
  CHECK_VARN(obj, EINVAL);
  CHECK_OBJSIZE(queue,objsize,EINVAL);

  INITIALIZE_NODE(tmp,queue,obj,flag);

  if (!S(queue)) {
    H(queue) = T(queue) = tmp;
    S(queue)++;
  } else {
    ADD_BACK(tmp, queue);
  }
  return SUCCESS;
allocobjfail:
  ADD_FREE_NODE(queue,tmp);
allocfail:
  return EALLOCF;
}

 int8_t
pop_QueueList(QueueList * queue)
{
  Node *tmp;

  CHECK_VARN(queue, EINVAL);

  if (!H(queue) || !S(queue)) {
    ERRORMSG(cannot pop front);
    return -1;
  }
  tmp = H(queue);
  REMOVE_FRONT(queue);
  DELETE_OBJPTR(queue,tmp);
  ADD_FREE_NODE(queue, tmp);
  return SUCCESS;
}

 void *
front_QueueList(QueueList * queue)
{
  CHECK_VARN(queue, NULL);
  CHECK_VARE(H(queue), NULL);
  return H(queue)->objptr;
}

F_CLEAR(QueueList) {
  PTR_CLEAR(QueueList,obj);
  return 0;
}

F_PRINT(QueueList) {
  Node *iter;
  CHECK_VARN(obj, EINVAL);
  CHECK_VARN(obj->API.print, ENOPRINT);
  FOR_EACH_NODE(iter, obj) {
    obj->API.print(iter->objptr);
  }
  return SUCCESS;
}

F_EMPTY(QueueList) {
	CHECK_VARN(obj, EEMPTY);
	CHECK_VARN(H(obj), EEMPTY);
	return !S(obj);
}

F_SIZE(QueueList) {
	CHECK_VARN(obj,0);
	return S(obj);
}
create_iter_func(Ptr_Based,QueueList)

function(size_of, QueueList)
function(set_compare, QueueList)
function(set_print, QueueList)
function(set_alloc, QueueList)
function(set_dealloc, QueueList)
function(set_copy, QueueList)
function(duplicate_ptr_struct,QueueList)

F_CONSTRUCT(QueueVector) {
	CHECK_VARN(obj,EINVAL);
	ARR_CONSTRUCT(QueueVector,obj,datasize,flag);
	return 0;
}

F_CONSTRUCT_FUNC(QueueVector) {
  CHECK_VARN(obj,EINVAL);
  ARR_STRUCT_SETUP(obj,datasize,flag);
  obj->API.alloc = alloc;
  obj->API.dealloc = dealloc;
  obj->API.cmp = cmp;
  obj->API.print = print;
  obj->API.copy = copy;
  return SUCCESS;
}

F_DESTRUCT(QueueVector) {
	CHECK_VARN(obj,EINVAL);
	clear(QueueVector,obj);
	free(M(obj));
	memset(obj,0,sizeof *obj);
	return 0;
}

int8_t pop_QueueVector(QueueVector *queue) {
	CHECK_VARN(queue,EINVAL);
	CHECK_VARN(H(queue),EINVAL);
	if(!S(queue)) {
		return 0;
	}

	ARR_POP_FRONT(QueueVector,queue);

	return 0;
}

void *front_QueueVector(QueueVector *queue) {
	CHECK_VARN(queue,NULL);
	CHECK_VARN(H(queue),NULL);
	return H(queue);
}

F_CLEAR(QueueVector) {
	CHECK_VARN(obj,EINVAL);
	if(!C(obj)) {
		return 0;
	}

	ARR_CLEAR(QueueVector,obj);

	return 0;
}

int8_t push_QueueVector(QueueVector *queue, void *obj, size_t objsize, int flag) {
	CHECK_VARN(queue,EINVAL);
	CHECK_VARN(obj,EINVAL);
	if(objsize != O(queue)) {
		flag = 0;
		return EINVAL;
	}

	ARR_PUSH_BACK(QueueVector,queue,obj,objsize);

	return 0;
}

int8_t resize_QueueVector(QueueVector *queue,size_t size) {
	void *ptr = NULL;
	CHECK_VARN(queue,EINVAL);
	CHECK_VARA(ptr = malloc(size * O(queue)),EALLOCF);
	
	ARR_COPY_WRAP(QueueVector,ptr,queue,size);

	ARR_SETUP_POINTERS(QueueVector,ptr,queue,size);
	return 0;
}
create_iter_func(Arr_Based,QueueVector)

function(size_of, QueueVector)
function(set_compare, QueueVector)
function(set_print, QueueVector)
function(set_alloc, QueueVector)
function(set_dealloc, QueueVector)
function(set_copy, QueueVector)
function(duplicate_arr_struct,QueueVector)


