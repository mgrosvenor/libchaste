/**
 * Copyright 2006, Micah Villmow, All Rights Reserved
 * @file control_macros.h
 * @brief Macros that help make the algorithms easier to
 * understand by abstracting the details
 */
#ifndef CONTROL_MACROS_H_
#define CONTROL_MACROS_H_
#include "access_macros.h"
#define LEFT 0x00
#define RIGHT 0x01

#ifdef VALID
#    undef VALID
#endif

#ifdef INVALID
#    undef INVALID
#endif

#define VALID 0x01
#define INVALID 0x00

#ifndef BASE_INIT
#    define BASE_INIT(X,Y,Z,S)	\
{\
	(Y)->API.copy((X)->objptr,(Z),(S));\
	N((X)) = P((X)) = NULL;\
}
#endif

#ifndef BASE_INIT2
#    define BASE_INIT2(X,Y,Z) \
{\
	(X)->objptr = (Z);\
	N((X)) = P((X)) = NULL;\
}
#endif

#ifndef DELETE_OBJPTR
#define DELETE_OBJPTR(PAR,OBJ) \
{\
  if(((PAR)->objfree == FREEOBJ || CHECK_FLAG((OBJ)->flags,STATIC)) && (OBJ)->objptr) {\
	  (PAR)->API.dealloc((OBJ)->objptr);\
	  (OBJ)->objptr = NULL;\
  }\
}
#endif

#ifndef ADD_FREE_NODE
#    define ADD_FREE_NODE(X,Y) \
{\
		  Node *__tmp;\
		  __tmp = FL((X));\
		  N((Y)) = __tmp;\
		  P((Y)) = NULL;\
		  if(__tmp) {\
		  P(__tmp) = (Y);\
		  }\
		  FL((X)) = (Y);\
}
#endif

#ifndef REMOVE_NODE
#    define REMOVE_NODE(X,Y) \
{\
	if(H(Y) == (X)) {\
		H(Y) = N(X);\
	}\
	if(T(Y) == (X)) {\
		T(Y) = P(X);\
	}\
	if(P((X))) {\
		N(P((X))) = N((X));\
	}\
	if(N((X))) {\
		P(N((X))) = P((X));\
	}\
	S(Y)--;\
}
#endif


#ifndef GET_FREE_NODE
#    define GET_FREE_NODE(X,Y) \
{\
		  (Y) = FL((X));\
		  FL((X)) = N(Y);\
}
#endif

#ifndef FOR_EACH_NODE
#    define FOR_EACH_NODE(X,Y) \
	for((X) = H((Y)); (X); (X) = N((X)))
#endif

#ifndef FOR_EACH_NODE_REVERSE
#    define FOR_EACH_NODE_REVERSE(X,Y)\
	for((X) = T((Y)); (X); (X) = P((X)))
#endif


#ifndef INITIALIZE_NODE
#define INITIALIZE_NODE(NEW,STRUCT,OBJ,FLAG) \
{\
	if(FL(STRUCT)) {\
		GET_FREE_NODE((STRUCT),(NEW));\
	} else if(!((NEW) = construct_Node(NUM_LINKS))) {\
		goto allocfail;\
	}\
	if((FLAG) == DYNAMIC) {\
		BASE_INIT2((NEW),(STRUCT),(OBJ));\
	} else {\
		if(!((NEW)->objptr = (STRUCT)->API.alloc((STRUCT)->objsize))) {\
			goto allocobjfail;\
		}\
		BASE_INIT((NEW),(STRUCT),(OBJ),(STRUCT)->objsize);\
	}\
	(NEW)->flags = (FLAG);\
	(NEW)->ptr[LEFT] = NULL;\
	(NEW)->ptr[RIGHT] = NULL;\
}
#endif

#ifndef TRAVERSE_TREE
#define TRAVERSE_TREE(PTR,TREE,NEW,RES,SIZE) \
{\
  do {\
	  (RES) = (TREE)->API.cmp((PTR)->objptr,(NEW),(SIZE));\
	  if((RES) > 0) {\
		  /* If the left child exists
		   * go to that child, otherwise
		   * assign the left child and
		   * break out of the loop
		   */\
		  if((PTR)->ptr[LEFT]) {\
			  (PTR) = (PTR)->ptr[LEFT];\
		  } else {\
			  break;\
		  }\
	  } else if((RES) < 0) {\
		  /* If the right child exists
		   * go to that child, otherwise
		   * assign the new object to
		   * the right child and return
		   */\
		  if((PTR)->ptr[RIGHT]) {\
			  (PTR) = (PTR)->ptr[RIGHT];\
		  } else {\
			  break;\
		  }\
	  } else {\
		  /* The node is the same as
		   * the current data being passed
		   * in, break out
		   */\
		  break;\
	  }\
  }while((PTR));\
}
#endif

#ifndef ADD_FRONT
#    define ADD_FRONT(X,Y) \
{\
	S((Y))++;\
	N((X)) = H((Y));\
	P((X)) = NULL;\
	P(H((Y))) = (X);\
	H((Y)) = (X);\
}
#endif



#ifndef ADD_BACK
#    define ADD_BACK(X,Y) \
{\
	S((Y))++;\
	P((X)) = T((Y));\
	N((X)) = NULL;\
	N(T((Y))) = (X);\
	T((Y)) = (X);\
}
#endif

#ifndef REMOVE_FRONT
#    define REMOVE_FRONT(X) \
{\
	S((X))--;\
	if(S((X))) { \
		H((X)) = N(H((X)));\
		P(H((X))) = NULL;\
	} else {\
		T((X)) = H((X)) = NULL;\
	}\
}
#endif

#ifndef REMOVE_BACK
#    define REMOVE_BACK(X) \
{\
	S((X))--;\
	if(S((X))) {\
		T((X)) = P(T((X)));\
		N(T((X))) = NULL;\
	} else { \
		T((X)) = H((X)) = NULL;\
	}\
}
#endif

#define PTR_CLEAR(TYPE,OBJ) \
do {\
	Node *iter, *iter2;\
	\
	CHECK_VARN((OBJ),EINVAL);\
	for(iter = H((OBJ));iter;iter = iter2) {\
		iter2 = N(iter);\
		REMOVE_NODE(iter,(OBJ));\
		DELETE_OBJPTR((OBJ),iter);\
		ADD_FREE_NODE((OBJ),iter);\
	}\
	H((OBJ)) = T((OBJ)) = NULL;\
	S((OBJ)) = 0;\
}while(0)


#ifndef PTR_STRUCT_SETUP
#define PTR_STRUCT_SETUP(X,Y,Z) \
	do{\
		int16_t x;\
		Node *ptr;\
		S((X)) = 0;\
		H((X)) = T((X)) = NULL;\
		(X)->objsize = (Y);\
		(X)->objfree = (Z);\
		for(x = 0; x < INITIAL_SIZE; x++) {\
			ptr = construct_Node(NUM_LINKS);\
			ADD_FREE_NODE((X),ptr);\
		}\
	}while(0)
#endif

#endif

