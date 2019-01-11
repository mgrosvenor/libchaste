/*
 * options_vectors.h
 *
 *  Created on: 16 Sep 2013
 *      Author: mpg39
 */

#ifndef OPTIONS_VECTORS_H_
#define OPTIONS_VECTORS_H_

#include "../data_structs/vector/vector_typed_declare_template.h"

typedef enum {
    CH_OPTION_REQUIRED,       //This option is required for the program to run
    CH_OPTION_OPTIONAL,       //This option is not required but has a default value
    CH_OPTION_UNLIMTED,       //This option produces a list of outputs
    CH_OPTION_FLAG,           //This option is optional, and has no arguments
} ch_options_mode_e;


typedef struct ch_options_opt{
    ch_options_mode_e mode;
    ch_types_e type;
    void* var;
    char short_opt;
    char* long_opt;
    char* descr;
    int found;
} ch_options_opt_t;


declare_ch_vector(opts, ch_options_opt_t)



#endif /* OPTIONS_VECTORS_H_ */
