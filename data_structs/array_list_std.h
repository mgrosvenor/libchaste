// CamIO 2: array_std_int.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef ARRAY_STD_INT_H_
#define ARRAY_STD_INT_H_

#include <stdint.h>
#include "array_list_declare_template.h"

typedef struct { ch_word x; ch_word y; } test_t;

declare_ch_array_list(test_t);
declare_ch_array_list(int);
declare_ch_array_list(short);
declare_ch_array_list(long);
declare_ch_array_list(char);
declare_ch_array_list(float);
declare_ch_array_list(double);
declare_ch_array_list(i64);





#endif /* ARRAY_STD_INT_H_ */
