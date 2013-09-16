/*
 * array_std.h
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */

#ifndef ARRAY_STD_H_
#define ARRAY_STD_H_

#include "array_typed_declare_template.h"

declare_ch_array(u8,  u8)
declare_ch_array(u16, u16)
declare_ch_array(u32, u32)
declare_ch_array(u64, u64)

declare_ch_array(u8p,  u8*)
declare_ch_array(u16p, u16*)
declare_ch_array(u32p, u32*)
declare_ch_array(u64p, u64*)

declare_ch_array(i8,  i8)
declare_ch_array(i16, i16)
declare_ch_array(i32, i32)
declare_ch_array(i64, i64)

declare_ch_array(i8p,  i8*)
declare_ch_array(i16p, i16*)
declare_ch_array(i32p, i32*)
declare_ch_array(i64p, i64*)

declare_ch_array(machine, ch_machine)
declare_ch_array(word, ch_word)
declare_ch_array(char, ch_char)
declare_ch_array(ch_bool, ch_bool)
declare_ch_array(float, ch_float)
declare_ch_array(string, ch_str)


declare_ch_array(machinep, ch_machine*)
declare_ch_array(wordp, ch_word*)
declare_ch_array(charp, ch_char*)
declare_ch_array(boolp, ch_bool*)
declare_ch_array(floatp, ch_float*)
declare_ch_array(stringp, ch_str*)


declare_ch_array(voidp,  void*)

#endif /* ARRAY_STD_H_ */
