
/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file error_macros.h
 * @author Micah Villmow
 * @brief Macros to handle error messages
 */
#ifndef ERROR_MACROS_H_
#    define ERROR_MACROS_H_
# define UNUSED __attribute__((__unused__))
#    define SUCCESS  0x00
#define EINVAL	0x02
#define ENOVAL	0x03
#    define EEMPTY  0x04
#    define EALLOCF  0x05
#    define ENOPRINT 0x06
#    define ENOCMP	 0x07
#define EOOB	0x01
#    define ENORCMP  0x08
#    define ENOALOC  0x10
#    define ENOFREE  0x20
#    define ENOCOPY	 0x30

#    ifndef ERRORINFO
#        define ERRORINFO  __FILE__,__func__,__LINE__
#    endif
#ifndef ERRORHDR
#define ERRORHDR	printf("%s:%s:%d - ",__FILE__,__func__,__LINE__);
#endif
#    ifndef NULLMSG
#        ifdef DEBUG
#            define NULLMSG(X) fprintf(stderr,"%s:%s:%d - "#X" pointer is NULL! Bad programming, fix this!\n", ERRORINFO)
#        else
#            define NULLMSG(X) do{}while(0)
#        endif
#    endif

#    ifndef ALLOCFAIL
#        ifdef DEBUG
#            define ALLOCFAIL(X) fprintf(stderr,"%s:%s:%d - allocation failed for "#X"\n", ERRORINFO)
#        else
#            define ALLOCFAIL(X) do{}while(0)
#        endif
#    endif

#    ifndef ERRORMSG
#        ifdef DEBUG
#            define ERRORMSG(X) fprintf(stderr,"%s:%s:%d - there exists an error "#X"\n", ERRORINFO)
#        else
#            define ERRORMSG(X) do{}while(0)
#        endif
#    endif
#ifndef NO_CHECKS
#    ifndef CHECK_VARN
#        define CHECK_VARN(X,Y)\
do {\
	if(!(X)) {\
		NULLMSG((X));\
		return (Y);\
	}\
}while(0)
#    endif

#    ifndef CHECK_VARE
#        define CHECK_VARE(X,Y)\
do {\
	if(!(X)) {\
		ERRORMSG((X));\
		return (Y);\
	}\
}while(0)
#    endif

#    ifndef CHECK_VARA
#        define CHECK_VARA(X,Y)\
do {\
	if(!(X)) {\
		ALLOCFAIL((X));\
		return (Y);\
	}\
}while(0)
#    endif
#else 
#ifndef CHECK_VARN
#define CHECK_VARN(X,Y)	if(0)
#endif
#ifndef CHECK_VARE
#define CHECK_VARE(X,Y) if(0)
#endif
#ifndef CEHCK_VARA
#define ChECK_VARA(X,Y) if(0)
#endif
#endif

#    ifndef CHECK_OBJSIZE
#        define CHECK_OBJSIZE(X,Y,Z) \
do {\
	if(O((X)) != (Y)) {\
		fprintf(stderr,"%s:%s:%d - Invalid object size(%d), should be %d\n",ERRORINFO,(unsigned)(Y),(unsigned)O((X)));\
		return (Z);\
	}\
}while(0)
#    endif


#endif
