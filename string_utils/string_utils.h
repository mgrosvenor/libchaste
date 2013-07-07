/*
 * string_utils.h
 *
 *  Created on: Jul 6, 2013
 *      Author: mgrosvenor
 */

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include "../deps/libbstring/bstrlib.h"
#include "../deps/libbstring/bstraux.h"

#include "../deps/libcgen/vector.h"


//Split and join functions, like python
Vector m6_bstring_split(bstring str, bstring delim );
bstring m6_bstring_join(Vector strs, bstring delim );



#endif /* STRING_UTILS_H_ */
