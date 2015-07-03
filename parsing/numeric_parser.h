/*
 * numeric_parser.h
 *
 *  Created on: Dec 20, 2012
 *      Author: mgrosvenor
 */

#ifndef NUMERIC_PARSER_H_
#define NUMERIC_PARSER_H_

#include <stdint.h>
#include <unistd.h>

#include "../types/types.h"

typedef struct{
    ch_types_e type;
    union{
        uint64_t val_uint;
        int64_t  val_int;
        double   val_dble;
    };
    ch_word index; //If the type returned is a valid number, return the index of the first char directly after the number
} num_result_t;


num_result_t parse_number(const char* c, size_t i);
num_result_t parse_nnumber(const char* c, size_t i, size_t n);


#endif /* NUMERIC_PARSER_H_ */
