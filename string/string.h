/*
 * string_utils.h
 *
 *  Created on: Jul 6, 2013
 *      Author: mgrosvenor
 */

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include "../types/types.h"
#include <string.h>


ch_str ch_str_free(ch_str* s); //Safe free function
i64 ch_str_resize(ch_str* rhs,i64 newsize); //
ch_str ch_str_new(const char* s,i64 size, ch_bool is_char); //Try not to use this, use the macro functions below which are better optimised for useful situations
ch_str ch_str_cat(ch_str* lhs, ch_str* rhs);
ch_str ch_str_cat_cstr(ch_str* lhs, const char* cstr);
ch_str ch_str_cat_char(ch_str* lhs, const char ch);
ch_str ch_str_trunc(ch_str* lhs, ch_word count);
ch_word ch_str_eq(ch_str lhs, ch_str rhs);



#define CH_STR_CONST_NEW(name, s) ch_str name = CH_STR_CONST_LIT(s)
#define CH_STR_CONST_LIT(s) { .cstr = (char*)s,    .is_const = 1,  .slen = strlen(s), .mlen = strlen(s) }
#define CH_STR_FUNC(s)      { .cstr = (char*)s,    .is_const = -1, .slen = -1,        .mlen = -1 }
#define CH_STR_NULL         { .cstr = NULL,        .is_const = 1, .slen = 0,        .mlen = 0 }
#define CH_STR(s,sz)  ch_str_new(s, sz, 0)

#define CH_STR_CAT(lhs,rhs)       ch_str_cat(lhs, rhs, 0)
#define CH_STR_CAT_CSTR(lhs,rhs)  ch_str_cat_cstr(lhs, rhs, 0)
#define CH_STR_CAT_CHAR(lhs,rhs)  ch_str_cat_char(lhs, rhs, 0)

#define CH_STR_CSTR(ch_str)     ( (ch_str).cstr)
#define CH_STR_AVAIL(ch_str)    ( (ch_str).mlen -1)
#define CH_STR_LEN(ch_str)      ( (ch_str).slen)


#endif /* STRING_UTILS_H_ */


