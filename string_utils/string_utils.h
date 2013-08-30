/*
 * string_utils.h
 *
 *  Created on: Jul 6, 2013
 *      Author: mgrosvenor
 */

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include "../types/types.h"


m6_str m6_str_free(m6_str s); //Safe free function
i64 m6_str_resize(m6_str* rhs,i64 newsize); //
m6_str m6_str_new(char* s,i64 size, m6_bool stack); //Try not to use this, use the macro functions below which are better optimised for useful situations
m6_str m6_str_cat(m6_str lhs, m6_str rhs, m6_bool stack);


#define M6_STR_CONST_NEW(name, s) m6_str name = M6_STR_CONST_LIT(s)
#define M6_STR_CONST_LIT(s) { .cstr = s,    .is_const = 1,  .is_stack = -1, .slen = strlen(s), .mlen = strlen(s) }
#define M6_STR_FUNC(s)      { .cstr = s,    .is_const = -1, .is_stack = -1, .slen = -1,        .mlen = -1 }
#define M6_STR_NULL         { .cstr = NULL, .is_const = -1, .is_stack = -1, .slen = -1,        .mlen = -1 }



#endif /* STRING_UTILS_H_ */
