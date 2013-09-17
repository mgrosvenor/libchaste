// CamIO 2: vector_std_int.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef VECTOR_STD_INT_H_
#define VECTOR_STD_INT_H_

#include <stdint.h>

#include "../../types/types.h"
#include "vector_typed_declare_template.h"

declare_ch_vector(u8,  u8)
declare_ch_vector(u16, u16)
declare_ch_vector(u32, u32)
declare_ch_vector(u64, u64)

declare_ch_vector(u8p,  u8*)
declare_ch_vector(u16p, u16*)
declare_ch_vector(u32p, u32*)
declare_ch_vector(u64p, u64*)

declare_ch_vector(i8,  i8)
declare_ch_vector(i16, i16)
declare_ch_vector(i32, i32)
declare_ch_vector(i64, i64)

declare_ch_vector(i8p,  i8*)
declare_ch_vector(i16p, i16*)
declare_ch_vector(i32p, i32*)
declare_ch_vector(i64p, i64*)

declare_ch_vector(machine, ch_machine)
declare_ch_vector(word, ch_word)
declare_ch_vector(char, ch_char)
declare_ch_vector(ch_bool, ch_bool)
declare_ch_vector(float, ch_float)
declare_ch_vector(string, ch_str)
declare_ch_vector(cstr, ch_cstr)

declare_ch_vector(machinep, ch_machine*)
declare_ch_vector(wordp, ch_word*)
declare_ch_vector(charp, ch_char*)
declare_ch_vector(boolp, ch_bool*)
declare_ch_vector(floatp, ch_float*)
declare_ch_vector(stringp, ch_str*)
declare_ch_vector(cstrp, ch_cstr*)

declare_ch_vector(voidp,  void*)

declare_ch_vector_cmp(u8,  u8)
declare_ch_vector_cmp(u16, u16)
declare_ch_vector_cmp(u32, u32)
declare_ch_vector_cmp(u64, u64)

declare_ch_vector_cmp(u8p,  u8*)
declare_ch_vector_cmp(u16p, u16*)
declare_ch_vector_cmp(u32p, u32*)
declare_ch_vector_cmp(u64p, u64*)

declare_ch_vector_cmp(i8,  i8)
declare_ch_vector_cmp(i16, i16)
declare_ch_vector_cmp(i32, i32)
declare_ch_vector_cmp(i64, i64)

declare_ch_vector_cmp(i8p,  i8*)
declare_ch_vector_cmp(i16p, i16*)
declare_ch_vector_cmp(i32p, i32*)
declare_ch_vector_cmp(i64p, i64*)

declare_ch_vector_cmp(machine, ch_machine)
declare_ch_vector_cmp(word, ch_word)
declare_ch_vector_cmp(char, ch_char)
declare_ch_vector_cmp(ch_bool, ch_bool)
declare_ch_vector_cmp(float, ch_float)
declare_ch_vector_cmp(string, ch_str)
declare_ch_vector_cmp(cstr, ch_cstr)

declare_ch_vector_cmp(machinep, ch_machine*)
declare_ch_vector_cmp(wordp, ch_word*)
declare_ch_vector_cmp(charp, ch_char*)
declare_ch_vector_cmp(boolp, ch_bool*)
declare_ch_vector_cmp(floatp, ch_float*)
declare_ch_vector_cmp(stringp, ch_str*)
declare_ch_vector_cmp(cstrp, ch_cstr*)

declare_ch_vector_cmp(voidp,  void*)




#endif /* ARRAY_STD_INT_H_ */
