/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Tools to parse and print command line options
 */

#ifndef CH_OPTIONS_H_
#define CH_OPTIONS_H_

#include "../types/types.h"
#include "../log/log.h" //Include this here, so users don't have to
#include "options_vectors.h"
#include "../data_structs/vector/vector_std.h"


typedef struct {
    char* short_description;
    char* long_description;
    ch_vector_t(ch_options_opt_t) opt_defs;
    uint64_t count;
    ch_bool help;
    int unlimted_set;
    ch_bool done_init;
} ch_options_t;


//Declare all the options parsers for non vector types, with initializers
#define ch_opt_add_declare_i(ch_type_name, c_type_name, short_name, long_name)\
int ch_opt_add##short_name##i(ch_options_mode_e mode, char short_str, char* long_str, char* descr, c_type_name* result_out, c_type_name default_val)
ch_opt_add_declare_i(CH_BOOL,     ch_bool,    b, "boolean");
ch_opt_add_declare_i(CH_UINT64,   u64,        u, "unsigned");
ch_opt_add_declare_i(CH_INT64,    ch_word,    i, "integer");
ch_opt_add_declare_i(CH_STRING,   ch_str,     s, "string");
ch_opt_add_declare_i(CH_DOUBLE,   ch_float,   f, "float");

//Declare all the options parsers for non vector types, untantalized
#define ch_opt_add_declare_u(ch_type_name, c_type_name, short_name, long_name)\
int ch_opt_add##short_name##u(ch_options_mode_e mode, char short_str, char* long_str, char* descr, c_type_name* result_out)
ch_opt_add_declare_u(CH_BOOL,     ch_bool,    b, "boolean");
ch_opt_add_declare_u(CH_UINT64,   u64,        u, "unsigned");
ch_opt_add_declare_u(CH_INT64,    ch_word,    i, "integer");
ch_opt_add_declare_u(CH_STRING,   ch_str,     s, "string");
ch_opt_add_declare_u(CH_DOUBLE,   ch_float,   f, "float");

//Declare all the options parsers for vector types, with initializers
#define ch_opt_add_declare_VI(ch_type_name, c_type_name_default, short_name, long_name)\
int ch_opt_add##short_name##I(ch_options_mode_e mode, char short_str, char* long_str, char* descr, ch_vector_t(c_type_name_default)* result_out, c_type_name_default default_val)
ch_opt_add_declare_VI(CH_BOOLS,    ch_bool,    B, "booleans");
ch_opt_add_declare_VI(CH_UINT64S,  u64,        U, "unsigneds");
ch_opt_add_declare_VI(CH_INT64S,   ch_word,    I, "integers");
ch_opt_add_declare_VI(CH_STRINGS,  ch_str,     S, "strings");
ch_opt_add_declare_VI(CH_DOUBLES,  ch_float,   F, "floats");

//Declare all the options parsers for vector types, with uninitialized
#define ch_opt_add_declare_VU(ch_type_name, c_type_name, short_name, long_name)\
int ch_opt_add##short_name##U(ch_options_mode_e mode, char short_str, char* long_str, char* descr, ch_vector_t(c_type_name)* result_out)
ch_opt_add_declare_VU(CH_BOOLS,    ch_bool,    B, "booleans");
ch_opt_add_declare_VU(CH_UINT64S,  u64,        U, "unsigneds");
ch_opt_add_declare_VU(CH_INT64S,   ch_word,    I, "integers");
ch_opt_add_declare_VU(CH_STRINGS,  ch_str,     S, "strings");
ch_opt_add_declare_VU(CH_DOUBLES,  ch_float,   F, "floats");

#define Vector_geti(type,vector,i) ((type*)vector.mem)[i]


#define USE_CH_OPTIONS \
    ch_options_t opts = { 0 }

int ch_opt_name(char* description);
int ch_opt_tail(char* description);
int ch_opt_short_description(char* description);
int ch_opt_long_description(char* description);
int ch_opt_parse(int argc, char** argv);


#endif /* CH_OPTIONS_H_ */
