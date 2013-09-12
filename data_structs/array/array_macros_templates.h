// CamIO 2: array_macros.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef ARRAY_MACROS_TEMPLATES_H_
#define ARRAY_MACROS_TEMPLATES_H_

#include "array.h"

#define ch_array(TYPE) ch_array_##TYPE

#define ch_array_new_declare(TYPE) \
  ch_array(TYPE)_t* ch_array_##TYPE##_new(ch_word size, ch_word(*cmp)(TYPE* lhs, TYPE* rhs));


typedef ch_word (*cmp_f)(void* lhs, void* rhs);

#define ch_array_new_define(TYPE) \
ch_array(TYPE)_t* ch_array_##TYPE##_new(ch_word size, ch_word (*cmp)(TYPE* lhs, TYPE* rhs)){ \
    return (ch_array(TYPE)_t*)ch_array_new( size, sizeof(TYPE), (cmp_f)cmp );\
}\



#endif /* ARRAY_MACROS_TEMPLATES_H_ */
