/*
 * array_std.c
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */



#include "array_std.h"
#include "array_typed_define_template.h"

define_array(u8,  u8)
define_array(u16, u16)
define_array(u32, u32)
define_array(u64, u64)

define_array(u8p,  u8*)
define_array(u16p, u16*)
define_array(u32p, u32*)
define_array(u64p, u64*)

define_array(i8,  i8)
define_array(i16, i16)
define_array(i32, i32)
define_array(i64, i64)

define_array(i8p,  i8*)
define_array(i16p, i16*)
define_array(i32p, i32*)
define_array(i64p, i64*)

define_array(machine, ch_machine)
define_array(word, ch_word)
define_array(char, ch_char)
define_array(ch_bool, ch_bool)
define_array(float, ch_float)
define_array(string, ch_str)


define_array(machinep, ch_machine*)
define_array(wordp, ch_word*)
define_array(charp, ch_char*)
define_array(boolp, ch_bool*)
define_array(floatp, ch_float*)
define_array(stringp, ch_str*)


define_array(voidp,  void*)
