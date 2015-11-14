/*
 * linked_list_std.h
 *
 *  Created on: Sep 23, 2013
 *      Author: mgrosvenor
 */

#ifndef LINKED_LIST_STD_H_
#define LINKED_LIST_STD_H_

#include "linked_list_typed_declare_template.h"

declare_ch_llist(u8,  u8)
declare_ch_llist(u16, u16)
declare_ch_llist(u32, u32)
declare_ch_llist(u64, u64)

declare_ch_llist(u8p,  u8*)
declare_ch_llist(u16p, u16*)
declare_ch_llist(u32p, u32*)
declare_ch_llist(u64p, u64*)

declare_ch_llist(i8,  i8)
declare_ch_llist(i16, i16)
declare_ch_llist(i32, i32)
declare_ch_llist(i64, i64)

declare_ch_llist(i8p,  i8*)
declare_ch_llist(i16p, i16*)
declare_ch_llist(i32p, i32*)
declare_ch_llist(i64p, i64*)

declare_ch_llist(machine, ch_machine)
declare_ch_llist(word, ch_word)
declare_ch_llist(char, ch_char)
declare_ch_llist(ch_bool, ch_bool)
declare_ch_llist(float, ch_float)
declare_ch_llist(string, ch_str)
declare_ch_llist(cstr, ch_cstr)

declare_ch_llist(machinep, ch_machine*)
declare_ch_llist(wordp, ch_word*)
declare_ch_llist(charp, ch_char*)
declare_ch_llist(boolp, ch_bool*)
declare_ch_llist(floatp, ch_float*)
declare_ch_llist(stringp, ch_str*)
declare_ch_llist(cstrp, ch_cstr*)

declare_ch_llist(voidp,  void*)

declare_ch_llist_cmp(u8,  u8);
declare_ch_llist_cmp(u16, u16);
declare_ch_llist_cmp(u32, u32);
declare_ch_llist_cmp(u64, u64);

declare_ch_llist_cmp(u8p,  u8*);
declare_ch_llist_cmp(u16p, u16*);
declare_ch_llist_cmp(u32p, u32*);
declare_ch_llist_cmp(u64p, u64*);

declare_ch_llist_cmp(i8,  i8);
declare_ch_llist_cmp(i16, i16);
declare_ch_llist_cmp(i32, i32);
declare_ch_llist_cmp(i64, i64);

declare_ch_llist_cmp(i8p,  i8*);
declare_ch_llist_cmp(i16p, i16*);
declare_ch_llist_cmp(i32p, i32*);
declare_ch_llist_cmp(i64p, i64*);

declare_ch_llist_cmp(machine, ch_machine);
declare_ch_llist_cmp(word, ch_word);
declare_ch_llist_cmp(char, ch_char);
declare_ch_llist_cmp(ch_bool, ch_bool);
declare_ch_llist_cmp(float, ch_float);
declare_ch_llist_cmp(string, ch_str);
declare_ch_llist_cmp(cstr, ch_cstr);

declare_ch_llist_cmp(machinep, ch_machine*);
declare_ch_llist_cmp(wordp, ch_word*);
declare_ch_llist_cmp(charp, ch_char*);
declare_ch_llist_cmp(boolp, ch_bool*);
declare_ch_llist_cmp(floatp, ch_float*);
declare_ch_llist_cmp(stringp, ch_str*);
declare_ch_llist_cmp(cstrp, ch_cstr*);

declare_ch_llist_cmp(voidp,  void*);


declare_ch_llist(UINT64, u64)
declare_ch_llist_cmp(UINT64, u64);

#endif /* LINKED_LIST_STD_H_ */
