/**
 * Copyright 2006, Micah Villmow, All Rights Reserved
 * @file control_arr_macros.h
 * @brief Macros that help in the modification of
 * array based structures
 */
#ifndef CONTROL_ARR_MACROS_H_
#define CONTROL_ARR_MACROS_H_
#include "access_macros.h"


#ifndef ARR_STRUCT_SETUP
#define ARR_STRUCT_SETUP(X,Y,Z) \
	do{\
	S((X)) = 0;\
	C((X)) = 0;\
	(X)->end = M((X)) = H((X)) = T((X)) = NULL;\
	(X)->objsize = (Y);\
	(X)->objfree = (Z);\
	}while(0)
#endif

#define ARR_CLEAR(TYPE,OBJ) \
do{ \
	H((OBJ)) = M((OBJ));\
	T((OBJ)) = ((char *)H((OBJ)) + O((OBJ)));\
	(OBJ)->end = (char *)H((OBJ)) + (O((OBJ)) * C((OBJ)));\
	S((OBJ)) = 0;\
}while(0)

#define ARR_POP_FRONT(TYPE,OBJ) \
do {\
	if(H((OBJ)) >= (OBJ)->end) {\
		H((OBJ)) = M((OBJ));\
	} else { \
		H((OBJ)) = ((char *)H((OBJ))) + O((OBJ));\
	}\
	S((OBJ))--;\
}while(0)

#define ARR_POP_BACK(TYPE,OBJ) \
do{\
	if(T((OBJ)) == M((OBJ))) {\
		T((OBJ)) = (OBJ)->end;\
	} else {\
		T((OBJ)) = ((char *)T((OBJ))) - O((OBJ));\
	}\
	S((OBJ))--;\
}while(0)

#define ARR_CONSTRUCT(TYPE,OBJ,DATASIZE,FLAG) \
do {\
	if(S((OBJ)) > 0) { \
		destruct(TYPE,(OBJ));\
	}\
	ARR_STRUCT_SETUP((OBJ),(DATASIZE),(FLAG));\
	API_DEFAULT_SETUP((OBJ));\
}while(0)

#define ARR_PUSH_BACK(TYPE,OBJ,ITEM,ITEMSIZE) \
do{ \
	if(S((OBJ)) == C((OBJ))) {\
		if(C((OBJ))) {\
			resize(TYPE,(OBJ),C((OBJ)) << 1);\
		} else {\
			CHECK_VARA(M((OBJ)) = malloc(O((OBJ)) * INITIAL_SIZE),EALLOCF);\
			H((OBJ)) = T((OBJ)) = M((OBJ));\
			C((OBJ)) = INITIAL_SIZE;\
			S((OBJ)) = 0;\
			(OBJ)->end = (char *)M((OBJ)) + (O((OBJ)) * C((OBJ)));\
		 }\
	}\
	if(T((OBJ)) >= (OBJ)->end) {\
		T((OBJ)) = M((OBJ));\
	}\
	(OBJ)->API.copy(T((OBJ)),(ITEM),(ITEMSIZE));\
	S((OBJ))++;\
	T((OBJ)) = ((char *)T((OBJ))) + O((OBJ));\
}while(0)

#define ARR_PUSH_FRONT(TYPE,OBJ,ITEM,ITEMSIZE) \
do{ \
	if(S((OBJ)) == C((OBJ))) {\
		if(C((OBJ))) {\
			resize(TYPE,(OBJ),C((OBJ)) << 1);\
		} else {\
			CHECK_VARA(M((OBJ)) = malloc(O((OBJ)) * INITIAL_SIZE),EALLOCF);\
			H((OBJ)) = T((OBJ)) = M((OBJ));\
			C((OBJ)) = INITIAL_SIZE;\
			S((OBJ)) = 0;\
			(OBJ)->end = (char *)M((OBJ)) + (O((OBJ)) * C((OBJ)));\
		 }\
	}\
	S((OBJ))++;\
	if(H((OBJ)) == M((OBJ))) {\
		H((OBJ)) = ((char *)(OBJ)->end) - O((OBJ));\
	} else {\
		H((OBJ)) = ((char *)H((OBJ))) - O((OBJ));\
	}\
    (OBJ)->API.copy(H((OBJ)),(ITEM),(ITEMSIZE));\
}while(0)


#define ARR_COPY_WRAP(TYPE,DST,SRC,SIZE) \
do {\
	if(M((SRC))) {\
	size_t off = S((SRC)) * O((SRC));\
	if(S((SRC)) == 0) {\
		/* header and tail are same, no need to copy */\
	} else if(H((SRC)) < T((SRC))) {\
		/* No wraparound */ \
		memcpy((DST),H((SRC)),((char *)T((SRC)) - (char *)H((SRC))));\
	} else if((void *)((char *)(H((SRC))) + off) < (SRC)->end) {\
		/* wraparound exists, but the new size fits between
		 * the current head and the end of the memarray*/\
		memcpy((DST),H((SRC)),off);\
	} else {\
		/* wraparound exists, and the new size
		 * overruns into the beginning, need to
		 * do a double copy
		 */\
		ptrdiff_t spaces = ((char *)(SRC)->end - (char *)H((SRC)));\
		memcpy((DST),H((SRC)),(size_t)spaces);\
		memcpy((char *)(DST) + spaces,M((SRC)),((O((SRC)) * (S((SRC))) - spaces/O((SRC)))));\
	}\
	free(M((SRC)));\
	}\
}while(0)

#define ARR_SETUP_POINTERS(TYPE,DST,SRC,SIZE) \
do{\
	M((SRC)) = H((SRC)) = (DST);\
	C((SRC)) = SIZE;\
	(SRC)->end = (char *)M((SRC)) + (O((SRC)) * C((SRC)));\
	if(S((SRC)) > C((SRC))) {\
	  S((SRC)) = C((SRC));\
	}\
	T((SRC)) = (char *)H((SRC)) + (S((SRC)) * O((SRC)));\
	if(T((SRC)) == H((SRC))) {\
		T((SRC)) = (char *)H((SRC)) + O((SRC));\
	}\
}while(0)

#endif
