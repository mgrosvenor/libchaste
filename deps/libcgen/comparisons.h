/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file comparisons.h
 * @brief File containing generic comparison functions
 * for most of the built in data types for C
 */

/**
 * @brief macro to expand to the comparison function
 * prototype
 */
#define CMPFUNC_PROTO(TYPE) int TYPE##cmp(void *, void *, size_t)
#define CMPFUNC_IMPL(TYPE)
	int TYPE##cmp(void *first, void *second, size_t size) {\
		return (*(const TYPE *)first) - (*(const TYPE *)second);\
	}


CMPFUNC_PROTO(char);
CMPFUNC_PROTO(float);
CMPFUNC_PROTO(double);
CMPFUNC_PROTO(int8_t);
CMPFUNC_PROTO(int16_t);
CMPFUNC_PROTO(int32_t);
CMPFUNC_PROTO(int64_t);
CMPFUNC_PROTO(uint8_t);
CMPFUNC_PROTO(uint16_t);
CMPFUNC_PROTO(uint32_t);
CMPFUNC_PROTO(uint64_t);

CMPFUNC_IMPL(char)
CMPFUNC_IMPL(float)
CMPFUNC_IMPL(double)
CMPFUNC_IMPL(int8_t)
CMPFUNC_IMPL(int16_t)
CMPFUNC_IMPL(int32_t)
CMPFUNC_IMPL(int64_t)
CMPFUNC_IMPL(uint8_t)
CMPFUNC_IMPL(uint16_t)
CMPFUNC_IMPL(uint32_t)
CMPFUNC_IMPL(uint64_t)

