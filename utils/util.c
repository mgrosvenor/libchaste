// CamIO 2: utils.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 
#include "../types/types.h"

//Thanks to http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
//Round up to the nearest power of 2
ch_word next_pow2(ch_word v){
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}
