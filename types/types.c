/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 */

#include "types.h"

ch_word is_vector(ch_word type){
    return type == CH_BOOLS   ||
           type == CH_INT64S  ||
           type == CH_STRINGS ||
           type == CH_UINT64S ||
           type == CH_DOUBLES;
}
