/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file conversions.h
 * @brief function macros to convert between two
 * distinct data types
 */
#include <string.h>

#include "queue.h"
#include "deque.h"
#include "list.h"
#include "gen/data_types.h"
#include "gen/gen_macros.h"
#include "vector.h"
#include "gen/error_macros.h"
#include "gen/access_macros.h"
#include "gen/function_macros.h"


#define convert_function(DST,SRC)\
	DST* to_##DST##_from_##SRC(SRC *src) {\
		DST *dst = NULL;\
		ITER(SRC)* iter = NULL;\
		void* tmpobj = NULL;\
		CHECK_VARN(src,NULL);\
		CHECK_VARA(dst = malloc(sizeof *dst),NULL);\
		memset(dst,0,sizeof *dst);\
		construct(DST,dst,dst->objsize,dst->objfree);\
		iter = create(SRC##Iter,src);\
		do {\
			tmpobj = retrieve(SRC##Iter,iter);\
			if(!tmpobj) {\
				destruct(DST,dst);\
				destroy(SRC##Iter,iter);\
				return NULL;\
			}\
			convert(DST,dst,tmpobj,O(src),DYNAMIC);\
		}while(next(SRC##Iter,iter));\
		destroy(SRC##Iter,iter);\
		return dst;\
	}

/*convert_function(List,QueueList)
convert_function(List,DequeList)
convert_function(List,Vector)

convert_function(QueueList,List)
convert_function(QueueList,DequeList)
convert_function(QueueList,Vector)

convert_function(DequeList,List)
convert_function(DequeList,QueueList)
convert_function(DequeList,Vector)

convert_function(Vector,List)
convert_function(Vector,QueueList)
convert_function(Vector,DequeList)*/
