/*
 * linked_list_std.c
 *
 *  Created on: Sep 23, 2013
 *      Author: mgrosvenor
 */

#include "linked_list.h"
#include "linked_list_std.h"
#include "linked_list_typed_define_template.h"
#include <string.h>

define_ch_llist(u8,  u8)
define_ch_llist(u16, u16)
define_ch_llist(u32, u32)
define_ch_llist(u64, u64)

define_ch_llist(u8p,  u8*)
define_ch_llist(u16p, u16*)
define_ch_llist(u32p, u32*)
define_ch_llist(u64p, u64*)

define_ch_llist(i8,  i8)
define_ch_llist(i16, i16)
define_ch_llist(i32, i32)
define_ch_llist(i64, i64)

define_ch_llist(i8p,  i8*)
define_ch_llist(i16p, i16*)
define_ch_llist(i32p, i32*)
define_ch_llist(i64p, i64*)

define_ch_llist(machine, ch_machine)
define_ch_llist(word, ch_word)
define_ch_llist(char, ch_char)
define_ch_llist(ch_bool, ch_bool)
define_ch_llist(float, ch_float)
define_ch_llist(string, ch_str)
define_ch_llist(cstr, ch_cstr)

define_ch_llist(machinep, ch_machine*)
define_ch_llist(wordp, ch_word*)
define_ch_llist(charp, ch_char*)
define_ch_llist(boolp, ch_bool*)
define_ch_llist(floatp, ch_float*)
define_ch_llist(stringp, ch_str*)
define_ch_llist(cstrp, ch_cstr*)

define_ch_llist(voidp,  void*)

define_ch_llist_cmp(u8,  u8)
define_ch_llist_cmp(u16, u16)
define_ch_llist_cmp(u32, u32)
define_ch_llist_cmp(u64, u64)

define_ch_llist_cmpp(u8,  u8*)
define_ch_llist_cmpp(u16, u16*)
define_ch_llist_cmpp(u32, u32*)
define_ch_llist_cmpp(u64, u64*)

define_ch_llist_cmp(i8,  i8)
define_ch_llist_cmp(i16, i16)
define_ch_llist_cmp(i32, i32)
define_ch_llist_cmp(i64, i64)


define_ch_llist_cmpp(i8,  i8*)
define_ch_llist_cmpp(i16, i16*)
define_ch_llist_cmpp(i32, i32*)
define_ch_llist_cmpp(i64, i64*)

define_ch_llist_cmp(machine, ch_machine)
define_ch_llist_cmp(word, ch_word)
define_ch_llist_cmp(char, ch_char)
define_ch_llist_cmp(ch_bool, ch_bool)
define_ch_llist_cmp(float, ch_float)

ch_word ch_llist_cmp_string(ch_str* lhs, ch_str* rhs)\
{
    return strcmp(lhs->cstr, rhs->cstr);
}

ch_word ch_llist_cmp_cstr(ch_cstr* lhs, ch_cstr* rhs)\
{
    return strcmp( (*lhs), (*rhs) ) ;
}


define_ch_llist_cmpp(machine, ch_machine*)
define_ch_llist_cmpp(word, ch_word*)
define_ch_llist_cmpp(char, ch_char*)
define_ch_llist_cmpp(ch_bool, ch_bool*)
define_ch_llist_cmpp(float, ch_float*)
define_ch_llist_cmpp(string, ch_str*)
define_ch_llist_cmpp(cstr, ch_cstr*)

define_ch_llist_cmp(void,  void*)
