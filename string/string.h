/*
 * string_utils.h
 *
 *  Created on: Jul 6, 2013
 *      Author: mgrosvenor
 */

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include "../types/types.h"


ch_str ch_str_free(ch_str s); //Safe free function
i64 ch_str_resize(ch_str* rhs,i64 newsize); //
ch_str ch_str_new(char* s,i64 size, ch_bool stack); //Try not to use this, use the macro functions below which are better optimised for useful situations
ch_str ch_str_cat(ch_str lhs, ch_str rhs, ch_bool stack);


#define CH_STR_CONST_NEW(name, s) ch_str name = CH_STR_CONST_LIT(s)
#define CH_STR_CONST_LIT(s) { .cstr = s,    .is_const = 1,  .is_stack = -1, .slen = strlen(s), .mlen = strlen(s) }
#define CH_STR_FUNC(s)      { .cstr = s,    .is_const = -1, .is_stack = -1, .slen = -1,        .mlen = -1 }
#define CH_STR_NULL         { .cstr = NULL, .is_const = -1, .is_stack = -1, .slen = -1,        .mlen = -1 }



#endif /* STRING_UTILS_H_ */
