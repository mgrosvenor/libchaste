/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Tools to parse and print command line options
 */

#ifndef CH_OPTIONS_H_
#define CH_OPTIONS_H_

#include "../types/types.h"
#include "../log/log.h" //Include this here, so users don't have to

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
    char short_str;
    char* long_str;
    char* descr;
    int found;
} ch_options_opt_t;


typedef struct {
    char* short_description;
    char* long_description;
    Vector opt_defs;
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
ch_opt_add_declare_i(CH_INT64,    i64,        i, "integer");
ch_opt_add_declare_i(CH_STRING,   char*,      s, "string");
ch_opt_add_declare_i(CH_DOUBLE,   double,     f, "float");

//Declare all the options parsers for non vector types, untantalized
#define ch_opt_add_declare_u(ch_type_name, c_type_name, short_name, long_name)\
int ch_opt_add##short_name##u(ch_options_mode_e mode, char short_str, char* long_str, char* descr, c_type_name* result_out)
ch_opt_add_declare_u(CH_BOOL,     ch_bool,    b, "boolean");
ch_opt_add_declare_u(CH_UINT64,   u64,        u, "unsigned");
ch_opt_add_declare_u(CH_INT64,    i64,        i, "integer");
ch_opt_add_declare_u(CH_STRING,   char*,      s, "string");
ch_opt_add_declare_u(CH_DOUBLE,   double,     f, "float");

//Declare all the options parsers for vector types, with initializers
#define ch_opt_add_declare_VI(ch_type_name, c_type_name_default, short_name, long_name)\
int ch_opt_add##short_name##I(ch_options_mode_e mode, char short_str, char* long_str, char* descr, Vector* result_out, c_type_name_default default_val)
ch_opt_add_declare_VI(CH_BOOLS,    ch_bool,    B, "booleans");
ch_opt_add_declare_VI(CH_UINT64S,  u64,        U, "unsigneds");
ch_opt_add_declare_VI(CH_INT64S,   i64,        I, "integers");
ch_opt_add_declare_VI(CH_STRINGS,  char*,      S, "strings");
ch_opt_add_declare_VI(CH_DOUBLES,  double,     F, "floats");

//Declare all the options parsers for vector types, with uninitialized
#define ch_opt_add_declare_VU(ch_type_name, c_type_name, short_name, long_name)\
int ch_opt_add##short_name##U(ch_options_mode_e mode, char short_str, char* long_str, char* descr, Vector* result_out)
ch_opt_add_declare_VU(CH_BOOLS,    ch_bool,    B, "booleans");
ch_opt_add_declare_VU(CH_UINT64S,  u64,        U, "unsigneds");
ch_opt_add_declare_VU(CH_INT64S,   i64,        I, "integers");
ch_opt_add_declare_VU(CH_STRINGS,  char*,      S, "strings");
ch_opt_add_declare_VU(CH_DOUBLES,  double,     F, "floats");

#define Vector_geti(type,vector,i) ((type*)vector.mem)[i]


#define USE_CH_OPTIONS \
    ch_options_t opts = { 0 }

int ch_opt_name(char* description);
int ch_opt_tail(char* description);
int ch_opt_short_description(char* description);
int ch_opt_long_description(char* description);
int ch_opt_parse(int argc, char** argv);


#endif /* CH_OPTIONS_H_ */
