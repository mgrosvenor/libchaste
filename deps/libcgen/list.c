
/**
 * Copyright 2006, Micah Villmow, All Rights Reserved
 * @file list.c
 * @author Micah Villmow
 * @brief a double linked list that handles any sized object
 */
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "list.h"
#include "gen/data_types.h"
#include "gen/error_macros.h"
#include "gen/access_macros.h"
#include "gen/control_macros.h"
#include "gen/function_macros.h"
#include "gen/function_signatures.h"

#ifdef NUM_LINKS
#undef NUM_LINKS
#endif
#define NUM_LINKS 2





#ifndef INSERT_NODE
#    define INSERT_NODE(X,Y,Z) \
{\
	if((Y) == H((Z))) {\
		H((Z)) = (X);\
	} \
	N(X) = (Y);\
	P(X) = P(Y);\
	P(Y) = (X);\
	if(P(X)) { \
		N(P(X)) = (X);\
	}\
	S(Z)++;\
}
#endif



#define SORT_LIST(W,X,Y,Z) \
{\
	int x = 0;\
	Node *iter;\
	FOR_EACH_NODE(iter,(X)) {\
		(X)->API.copy(&(W)[x++ * (Z)],(char *)iter->objptr,(Z));\
	}\
	qsort((W),S((X)),(Z),(Y));\
	x = 0;\
	FOR_EACH_NODE(iter,(X)) {\
		(X)->API.copy((char *)iter->objptr,&(W)[x++ * (Z)],(Z));\
	}\
}



/* See header file for comments for all functions */
F_CONSTRUCT(List) {
  CHECK_VARN(obj, EINVAL);
  if (S(obj)) {
    destruct(List, obj);
  }
  PTR_STRUCT_SETUP(obj,datasize,flag);
  API_DEFAULT_SETUP(obj);
  return 0;
}

F_CONSTRUCT_FUNC(List) {
  PTR_STRUCT_SETUP(obj,datasize,flag);
  obj->API.alloc = alloc;
  obj->API.dealloc = dealloc;
  obj->API.cmp = cmp;
  obj->API.print = print;
  obj->API.copy = copy;
  return 0;
}

F_DESTRUCT(List) {
  Node *tmp, *ptr;
  CHECK_VARN(obj, EINVAL);
  clear(List, obj);
  for (ptr = FL(obj); ptr; ptr = tmp) {
    tmp = N(ptr);
    destruct(Node, ptr);
  }
  memset(obj,0,sizeof *obj);
  return 0;
}

F_CLEAR(List) {
  PTR_CLEAR(List,obj);
  return 0;
}

int8_t
push_front_List(List * list, void *obj, size_t objsize, int flag)
{
  Node *tmp;

  CHECK_VARN(list, EINVAL);
  CHECK_VARN(obj, EINVAL);
  CHECK_OBJSIZE(list,objsize,EINVAL);

  INITIALIZE_NODE(tmp,list,obj,flag);

  if (!S(list)) {
    H(list) = T(list) = tmp;
    S(list)++;
  } else {
    ADD_FRONT(tmp, list);
  }

  return SUCCESS;
allocobjfail:
  ADD_FREE_NODE(list,tmp);
allocfail:
  return -EALLOCF;
}

int8_t
push_back_List(List * list, void *obj, size_t objsize, int flag)
{
  Node *tmp;

  CHECK_VARN(list, EINVAL);
  CHECK_VARN(obj, EINVAL);
  CHECK_OBJSIZE(list,objsize,EINVAL);

  INITIALIZE_NODE(tmp,list,obj,flag);

  if (!S(list)) {
    H(list) = T(list) = tmp;
    S(list)++;
  } else {
    ADD_BACK(tmp, list);
  }
  return SUCCESS;
allocobjfail:
  ADD_FREE_NODE(list,tmp);
allocfail:
  return EALLOCF;
}

int8_t
pop_front_List(List * list)
{
  Node *tmp;

  CHECK_VARN(list, EINVAL);

  if (!H(list) || !S(list)) {
    ERRORMSG(cannot pop front);
    return -1;
  }
  tmp = H(list);
  REMOVE_FRONT(list);
  DELETE_OBJPTR(list,tmp);
  ADD_FREE_NODE(list, tmp);
  return SUCCESS;
}

int8_t
pop_back_List(List * list)
{
  Node *tmp;

  CHECK_VARN(list, EINVAL);

  if (!T(list) || !S(list)) {
    ERRORMSG(cannot pop back);
    return -1;
  }
  tmp = T(list);
  REMOVE_BACK(list);
  DELETE_OBJPTR(list,tmp);
  ADD_FREE_NODE(list, tmp);
  return SUCCESS;
}

void *
front_List(List * list)
{
  CHECK_VARN(list, NULL);
  CHECK_VARE(H(list), NULL);
  return H(list)->objptr;
}

void *
back_List(List * list)
{
  CHECK_VARN(list, NULL);
  CHECK_VARE(T(list), NULL);
  return T(list)->objptr;
}

void *
insert_List(List * list, void *obj, size_t objsize, int flag)
{
  Node *iter, *tmp;

  CHECK_VARN(list, NULL);
  CHECK_VARN(obj, NULL);
  CHECK_VARE(objsize, NULL);
  CHECK_OBJSIZE(list,objsize,NULL);

  INITIALIZE_NODE(tmp,list,obj,flag);

  if (!S(list)) {
    H(list) = T(list) = tmp;
    S(list)++;
  } else {
  FOR_EACH_NODE(iter, list) {
	if (list->API.cmp(iter->objptr, obj,objsize) >= 0) {
	  break;
	}
  }
    if (!iter) {
      N(tmp) = NULL;
      P(tmp) = T(list);
      N(T(list)) = tmp;
      T(list) = tmp;
      S(list)++;
    } else {
      INSERT_NODE(tmp, iter, list);
    }
  }
  return tmp;
allocobjfail:
  ADD_FREE_NODE(list,tmp);
allocfail:
  ALLOCFAIL(object);
  return NULL;

}
/*List* duplicate_List(List* src) {
	List* dst;
	Node* iter, *tmp;
	int x;
		CHECK_VARN(src,NULL);
		CHECK_VARA((dst = malloc(sizeof *dst)),NULL);
		dst->objfree = FREEOBJ;
		dst->objsize = src->objsize;
		dst->API.alloc = src->API.alloc;
		dst->API.dealloc = src->API.dealloc;
		dst->API.cmp = src->API.cmp;
		dst->API.print = src->API.print;
		dst->API.copy = src->API.copy;
		dst->size = 0;
		FL(dst) = H(dst) = T(dst) = NULL;
		for(x = 0; x < (INITIAL_SIZE + src->size); x++) {
			iter = construct_Node(NUM_LINKS);
			N(iter) = P(iter) = NULL;
			ADD_FREE_NODE(dst,iter);
		}
		for(iter = H(src); iter; iter = N(iter)) {
			INITIALIZE_NODE(tmp,dst,iter->objptr,STATIC);
			if(!S(dst)) {
				H(dst) = T(dst) = tmp;
				S(dst)++;
			} else {
				ADD_BACK(tmp,dst);
			}
		}
		return dst;
		allocobjfail:
		allocfail:
		destruct(List,dst);
		return NULL;
}*/
int8_t
delete_List(List * list, void *obj, size_t objsize)
{
  Node *iter;

  CHECK_VARN(list, EINVAL);
  CHECK_VARN(obj, EINVAL);
  CHECK_VARE(objsize, EINVAL);
  CHECK_VARE(S(list), EEMPTY);

    FOR_EACH_NODE(iter, list) {
      if (!list->API.cmp(iter->objptr, obj,objsize)) {
        break;
      }
    }
  if (iter) {
    REMOVE_NODE(iter, list);
    DELETE_OBJPTR(list,iter);
    ADD_FREE_NODE(list, iter);
  }
  return SUCCESS;
}

void *
find_List(List * list, void *obj, size_t objsize)
{
  Node *iter;

  CHECK_VARN(list, NULL);
  CHECK_VARN(obj, NULL);
  if (!S(list)) {
    return NULL;
  }
	FOR_EACH_NODE_REVERSE(iter, list) {
	  if (!list->API.cmp(iter->objptr, obj,objsize)) {
		return iter->objptr;
	  }
	}
  return NULL;
}

int8_t
sort_List(List * list, size_t objsize)
{
  char *array;
  int   x = 0;
  Node *iter;

  CHECK_VARN(list, EINVAL);
  CHECK_VARE(objsize, EINVAL);
  CHECK_VARE(list->API.cmp, ENOCMP);
  CHECK_VARA((array = malloc(objsize * S(list))), -1);

  FOR_EACH_NODE(iter, list) {
    list->API.copy(&array[x++ * objsize], iter->objptr, objsize);
  }
  qsort(array, S(list), objsize, (__compar_fn_t)list->API.cmp);
  x = 0;
  FOR_EACH_NODE(iter, list) {
    list->API.copy(iter->objptr, &array[x++ * objsize], objsize);
  }
  free(array);
  return SUCCESS;
}
/*
int8_t
rsort_List(List * list, size_t objsize)
{
  char *array[2];
  uint32_t x[2];
  Node *iter;

  CHECK_VARN(list, EINVAL);
  CHECK_VARE(objsize, EINVAL);
  CHECK_VARE(list->API.rcmp, ENORCMP);
  CHECK_VARA((array[0] = malloc(objsize * S(list))), -1);

  x[0] = 0;
  if (!(array[1] = malloc(objsize * S(list)))) {
    ALLOCFAIL(array[1]);
    free(array[0]);
    return -EALLOCF;
  }
  FOR_EACH_NODE(iter, list) {
    list->API.copy(&array[0][x[0]++ * objsize], iter->objptr, objsize);
  }
  qsort(array[0], S(list), objsize, list->API.rcmp);
  for (x[0] = 0, x[1] = S(list) - 1; x[0] < S(list); x[0]++, x[1]--) {
    list->API.copy(&array[1][x[1] * objsize], &array[0][x[0] * objsize],
                   objsize);
  }
  x[1] = 0;
  FOR_EACH_NODE(iter, list) {
    list->API.copy((char *) iter->objptr, &array[1][x[1]++ * objsize],
                   objsize);
  }
  free(array[0]);
  free(array[1]);
  return SUCCESS;
}*/

F_PRINT(List) {
  Node *iter;
  CHECK_VARN(obj, EINVAL);
  CHECK_VARN(obj->API.print, ENOPRINT);
  FOR_EACH_NODE(iter, obj) {
    obj->API.print(iter->objptr);
  }
  return SUCCESS;
}

F_EMPTY(List) {
	CHECK_VARN(obj, EEMPTY);
	CHECK_VARN(H(obj), EEMPTY);
	return !S(obj);
}

F_SIZE(List) {
	CHECK_VARN(obj,0);
	return S(obj);
}


void *
to_array_List(List * list, size_t objsize)
{
  char *array;
  uint32_t x;
  Node *iter;

  CHECK_VARN(list, NULL);
  CHECK_VARE(objsize, NULL);
  CHECK_VARA(array = malloc(objsize * S(list)), NULL);
  x = 0;
  FOR_EACH_NODE(iter, list) {
    list->API.copy(&array[x++ * objsize], iter->objptr, objsize);
  }
  return array;
}

ITER(List) * extract_ListIter(ITER(List) * iter)
{
  Node *tmp;
  Node *node;
  List *list;

  CHECK_VARN(iter, iter);
  list = iter->parent;
  node = iter->ptr;
  if (0 == S(list)) {
    return iter;
  }
  if (S(list) == 1) {
    S(list)--;
    H(list) = T(list) = NULL;
    DELETE_OBJPTR(list,node);
    ADD_FREE_NODE(list,node);
    return iter;
  }
  tmp = N(node);
  if (H(list) == node) {
    /*
     * Special case if node is head of list 
     */
    H(list) = tmp;
    P(tmp) = NULL;
  } else if (T(list) == node) {
    /*
     * Special case if node is tail of list 
     */
    T(list) = P(node);
    N(T(list)) = NULL;
  } else {
    /*
     * Generic case where node is in middle of list 
     */
    N(P(node)) = tmp;
    P(tmp) = P(node);
  }
  S(list)--;
  DELETE_OBJPTR(list,node);
  ADD_FREE_NODE(list,node);
  iter->ptr = tmp;
  return iter;
}

create_iter_func(Ptr_Based,List)
function(duplicate_ptr_struct,List)
function(size_of, List)
function(set_compare, List)
function(set_print, List)
function(set_copy, List)
function(set_alloc, List)
function(set_dealloc, List)

#ifdef DEBUG
F_DUMP(List) {
  Node *iter;

  CHECK_VARN(obj, EINVAL);
  /*printf("cmp = %p, print = %p, size = %d\n", (void *) list->API.cmp,
         (void *) list->print, (int) S(list));*/
  FOR_EACH_NODE(iter, obj) {
    printf("Prev = %p, Node = %p, Next = %p\n", (void *) P(iter),
           (void *) iter, (void *) N(iter));
  }
  printf("\n");
  return SUCCESS;
}


#endif
