/*
 * array_std_int.c
 *
 *  Created on: 6 Sep 2013
 *      Author: mpg39
 */

#include "../../types/types.h"
#include "vector.h"
#include "vector_typed_define_template.h"
#include <string.h>

define_ch_vector(u8,  u8)
define_ch_vector(u16, u16)
define_ch_vector(u32, u32)
define_ch_vector(u64, u64)

define_ch_vector(u8p,  u8*)
define_ch_vector(u16p, u16*)
define_ch_vector(u32p, u32*)
define_ch_vector(u64p, u64*)

define_ch_vector(i8,  i8)
define_ch_vector(i16, i16)
define_ch_vector(i32, i32)
define_ch_vector(i64, i64)

define_ch_vector(i8p,  i8*)
define_ch_vector(i16p, i16*)
define_ch_vector(i32p, i32*)
define_ch_vector(i64p, i64*)

define_ch_vector(machine, ch_machine)
define_ch_vector(word, ch_word)
define_ch_vector(char, ch_char)
define_ch_vector(ch_bool, ch_bool)
define_ch_vector(float, ch_float)
define_ch_vector(string, ch_str)
define_ch_vector(cstr, ch_cstr)

define_ch_vector(machinep, ch_machine*)
define_ch_vector(wordp, ch_word*)
define_ch_vector(charp, ch_char*)
define_ch_vector(boolp, ch_bool*)
define_ch_vector(floatp, ch_float*)
define_ch_vector(stringp, ch_str*)
define_ch_vector(cstrp, ch_cstr*)

define_ch_vector(voidp,  void*)

define_ch_vector_cmp(u8,  u8)
define_ch_vector_cmp(u16, u16)
define_ch_vector_cmp(u32, u32)
define_ch_vector_cmp(u64, u64)

define_ch_vector_cmpp(u8,  u8*)
define_ch_vector_cmpp(u16, u16*)
define_ch_vector_cmpp(u32, u32*)
define_ch_vector_cmpp(u64, u64*)

define_ch_vector_cmp(i8,  i8)
define_ch_vector_cmp(i16, i16)
define_ch_vector_cmp(i32, i32)
define_ch_vector_cmp(i64, i64)

define_ch_vector_cmpp(i8,  i8*)
define_ch_vector_cmpp(i16, i16*)
define_ch_vector_cmpp(i32, i32*)
define_ch_vector_cmpp(i64, i64*)

define_ch_vector_cmp(machine, ch_machine)
define_ch_vector_cmp(word, ch_word)
define_ch_vector_cmp(char, ch_char)
define_ch_vector_cmp(ch_bool, ch_bool)
define_ch_vector_cmp(float, ch_float)

ch_word ch_vector_cmp_string(ch_str* lhs, ch_str* rhs)\
{
    return strcmp(lhs->cstr, rhs->cstr);
}

ch_word ch_vector_cmp_cstr(ch_cstr* lhs, ch_cstr* rhs)\
{
    return strcmp( (*lhs), (*rhs) ) ;
}


define_ch_vector_cmpp(machine, ch_machine*)
define_ch_vector_cmpp(word, ch_word*)
define_ch_vector_cmpp(char, ch_char*)
define_ch_vector_cmpp(ch_bool, ch_bool*)
define_ch_vector_cmpp(float, ch_float*)
define_ch_vector_cmpp(string, ch_str*)
define_ch_vector_cmpp(cstr, ch_cstr*)

define_ch_vector_cmp(void,  void*)
