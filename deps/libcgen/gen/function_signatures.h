/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file function_signatures.h
 * @brief Macros for function signatures that are
 * used throughout the generic function library
 */

#ifndef FUNCTION_SIGNATURES_H_
#define FUNCTION_SIGNATURES_H_

#ifndef F_CONSTRUCT
#define F_CONSTRUCT(TYPE)\
	int8_t construct_##TYPE(TYPE* obj, size_t datasize, int flag)
#endif

#ifndef F_CONSTRUCT_FUNC
#define F_CONSTRUCT_FUNC(TYPE)\
	int8_t construct_func_##TYPE(TYPE* obj, size_t datasize, int flag,\
                             void *(*alloc) (size_t),\
                             void (*dealloc) (void *),\
                             int32_t (*cmp) (const void *, const void *,size_t),\
                             void (*print) (const void *),\
                             void *(*copy) (void *, const void *, size_t))
#endif


#ifndef F_DESTRUCT
#define F_DESTRUCT(TYPE)\
	int8_t destruct_##TYPE(TYPE* obj)
#endif

#ifndef F_CLEAR
#define F_CLEAR(TYPE)\
	int8_t clear_##TYPE(TYPE* obj)
#endif

#ifndef F_DUPLICATE
#define F_DUPLICATE(TYPE)\
	TYPE* duplicate_##TYPE(TYPE* obj)
#endif

#ifndef F_PRINT
#define F_PRINT(TYPE)\
	int8_t print_##TYPE(TYPE* obj)
#endif

#ifndef F_EMPTY
#define F_EMPTY(TYPE)\
	int8_t empty_##TYPE(TYPE* obj)
#endif

#ifndef F_SIZE
#define F_SIZE(TYPE)\
	size_t size_##TYPE(TYPE* obj)
#endif

#ifndef F_DUMP
#define F_DUMP(TYPE)\
	int8_t dump_##TYPE(TYPE* obj)
#endif

#ifndef F_SIZE_OF
#define F_SIZE_OF(TYPE)\
	size_t size_of_##TYPE(TYPE* obj)
#endif

#ifndef F_SET_COMPARE
#define F_SET_COMPARE(TYPE)\
	int8_t set_compare_##TYPE(TYPE* obj, int32_t (*cmp) (const void *, const void *, size_t))
#endif

#ifndef F_SET_PRINT
#define F_SET_PRINT(TYPE)\
	int8_t set_print_##TYPE(TYPE* obj, void (*print) (const void *))
#endif	


#ifndef F_SET_ALLOC
#define F_SET_ALLOC(TYPE)\
	int8_t set_alloc_##TYPE(TYPE* obj, void* (*alloc) (size_t))
#endif

#ifndef F_SET_DEALLOC
#define F_SET_DEALLOC(TYPE)\
	int8_t set_dealloc_##TYPE(TYPE* obj, void (*dealloc) (void *))
#endif

#ifndef F_SET_COPY
#define F_SET_COPY(TYPE)\
	int8_t set_copy_##TYPE(TYPE* obj, void *(*copy)(void *, const void *, size_t))
#endif

#endif
