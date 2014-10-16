/*
 * array_std.c
 *
 *  Created on: Sep 12, 2013
 *      Author: mgrosvenor
 */

#include <string.h>

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

define_ch_array_cmp(u8,  u8)
define_ch_array_cmp(u16, u16)
define_ch_array_cmp(u32, u32)
define_ch_array_cmp(u64, u64)

define_ch_array_cmpp(u8,  u8*)
define_ch_array_cmpp(u16, u16*)
define_ch_array_cmpp(u32, u32*)
define_ch_array_cmpp(u64, u64*)

define_ch_array_cmp(i8,  i8)
define_ch_array_cmp(i16, i16)
define_ch_array_cmp(i32, i32)
define_ch_array_cmp(i64, i64)

define_ch_array_cmpp(i8,  i8*)
define_ch_array_cmpp(i16, i16*)
define_ch_array_cmpp(i32, i32*)
define_ch_array_cmpp(i64, i64*)

define_ch_array_cmp(machine, ch_machine)
define_ch_array_cmp(word, ch_word)
define_ch_array_cmp(char, ch_char)
define_ch_array_cmp(ch_bool, ch_bool)
define_ch_array_cmp(float, ch_float)

ch_word ch_array_cmp_string(ch_str* lhs, ch_str* rhs)\
{
    return strcmp(lhs->cstr, rhs->cstr);
}

ch_word ch_array_cmp_cstr(ch_cstr* lhs, ch_cstr* rhs)\
{
    return strcmp( (*lhs), (*rhs) ) ;
}


define_ch_array_cmpp(machine, ch_machine*)
define_ch_array_cmpp(word, ch_word*)
define_ch_array_cmpp(char, ch_char*)
define_ch_array_cmpp(ch_bool, ch_bool*)
define_ch_array_cmpp(float, ch_float*)
define_ch_array_cmpp(string, ch_str*)
define_ch_array_cmpp(cstr, ch_cstr*)
