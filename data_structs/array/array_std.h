/*
 * array_std.h
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */

#ifndef ARRAY_STD_H_
#define ARRAY_STD_H_

#include "array_typed_declare_template.h"

declare_array(u8,  u8)
declare_array(u16, u16)
declare_array(u32, u32)
declare_array(u64, u64)

declare_array(u8p,  u8*)
declare_array(u16p, u16*)
declare_array(u32p, u32*)
declare_array(u64p, u64*)

declare_array(i8,  i8)
declare_array(i16, i16)
declare_array(i32, i32)
declare_array(i64, i64)

declare_array(i8p,  i8*)
declare_array(i16p, i16*)
declare_array(i32p, i32*)
declare_array(i64p, i64*)

declare_array(machine, ch_machine)
declare_array(word, ch_word)
declare_array(char, ch_char)
declare_array(ch_bool, ch_bool)
declare_array(float, ch_float)
declare_array(string, ch_str)


declare_array(machinep, ch_machine*)
declare_array(wordp, ch_word*)
declare_array(charp, ch_char*)
declare_array(boolp, ch_bool*)
declare_array(floatp, ch_float*)
declare_array(stringp, ch_str*)

declare_array(voidp,  void*)

declare_ch_array_cmp(u8,  u8)
declare_ch_array_cmp(u16, u16)
declare_ch_array_cmp(u32, u32)
declare_ch_array_cmp(u64, u64)

declare_ch_array_cmp(u8p,  u8*)
declare_ch_array_cmp(u16p, u16*)
declare_ch_array_cmp(u32p, u32*)
declare_ch_array_cmp(u64p, u64*)

declare_ch_array_cmp(i8,  i8)
declare_ch_array_cmp(i16, i16)
declare_ch_array_cmp(i32, i32)
declare_ch_array_cmp(i64, i64)

declare_ch_array_cmp(i8p,  i8*)
declare_ch_array_cmp(i16p, i16*)
declare_ch_array_cmp(i32p, i32*)
declare_ch_array_cmp(i64p, i64*)

declare_ch_array_cmp(machine, ch_machine)
declare_ch_array_cmp(word, ch_word)
declare_ch_array_cmp(char, ch_char)
declare_ch_array_cmp(ch_bool, ch_bool)
declare_ch_array_cmp(float, ch_float)
declare_ch_array_cmp(string, ch_str)
declare_ch_array_cmp(cstr, ch_cstr)

declare_ch_array_cmp(machinep, ch_machine*)
declare_ch_array_cmp(wordp, ch_word*)
declare_ch_array_cmp(charp, ch_char*)
declare_ch_array_cmp(boolp, ch_bool*)
declare_ch_array_cmp(floatp, ch_float*)
declare_ch_array_cmp(stringp, ch_str*)
declare_ch_array_cmp(cstrp, ch_cstr*)

declare_ch_array_cmp(voidp,  void*)

#endif /* ARRAY_STD_H_ */
