// CamIO 2: array_list_std_int.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef ARRAY_LIST_STD_INT_H_
#define ARRAY_LIST_STD_INT_H_

#include <stdint.h>

#include "../../types/types.h"
#include "array_list_declare_template.h"

declare_ch_array_list(int);
declare_ch_array_list(short);
declare_ch_array_list(long);
declare_ch_array_list(char);
declare_ch_array_list(float);
declare_ch_array_list(double);

declare_ch_array_list(u8);
declare_ch_array_list(u16);
declare_ch_array_list(u32);
declare_ch_array_list(u64);

declare_ch_array_list(i8);
declare_ch_array_list(i16);
declare_ch_array_list(i32);
declare_ch_array_list(i64);

declare_ch_array_list(ch_machine);
declare_ch_array_list(ch_word);
declare_ch_array_list(ch_char);
declare_ch_array_list(ch_bool);


#endif /* ARRAY_STD_INT_H_ */
