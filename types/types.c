/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 */

#include "types.h"

m6_word is_vector(m6_word type){
    return type == M6_BOOLS   ||
           type == M6_INT64S  ||
           type == M6_STRINGS ||
           type == M6_UINT64S ||
           type == M6_DOUBLES;
}
