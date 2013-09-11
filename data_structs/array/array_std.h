// CamIO 2: array_std_int.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef ARRAY_STD_INT_H_
#define ARRAY_STD_INT_H_

#include <stdint.h>
#include "array_declare_template.h"

declare_ch_array(int);
declare_ch_array(short);
declare_ch_array(long);
declare_ch_array(char);
declare_ch_array(float);
declare_ch_array(double);

declare_ch_array(u8);
declare_ch_array(u16);
declare_ch_array(u32);
declare_ch_array(u64);

declare_ch_array(i8);
declare_ch_array(i16);
declare_ch_array(i32);
declare_ch_array(i64);

declare_ch_array(ch_machine);
declare_ch_array(ch_word);
declare_ch_array(ch_char);
declare_ch_array(ch_bool);



#endif /* ARRAY_STD_INT_H_ */
