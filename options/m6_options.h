/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Tools to parse and print command line options
 */

#ifndef M6_OPTIONS_H_
#define M6_OPTIONS_H_

#include "../types/types.h"


typedef enum {
    M6_OPTION_REQUIRED,       //This option is required for the program to run
    M6_OPTION_OPTIONAL,       //This option is not required but has a default value
    M6_OPTION_UNLIMTED,       //This option produces a list of outputs
    M6_OPTION_FLAG,           //This option is optional, and has no arguments
} m6_options_mode_e;



typedef struct m6_options_opt{
    m6_options_mode_e mode;
    m6_types_e type;
    void* var;
    char short_str;
    const char* long_str;
    const char* descr;
    int found;
} m6_options_opt_t;


typedef struct {
    char* short_description;
    char* long_description;
    Vector opt_defs;
    uint64_t count;
    m6_bool help;
    int unlimted_set;
    m6_bool done_init;
} m6_options_t;


#define m6_options_add_declare(m6_type_name, c_type_name, short_name, long_name)\
inline int m6_options_add##short_name(m6_options_mode_e mode, char short_str, const char* long_str, const char* descr, c_type_name* result_out, c_type_name default_val) \

#define m6_options_add_define(m6_type_name, c_type_name, short_name, long_name)\
m6_options_add_declare(6_type_name, c_type_name, short_name, long_name)\
{\
    m6_options_opt_t opt_new = {0};\
    *result_out = default_val;\
    m6_word result = m6_options_add_init(&opt_new, mode, short_str, long_str, descr, m6_type_name, result_out);\
    if(result){\
        return result;\
    }\
    if(push_back(Vector,&opts.opt_defs, &opt_new,STATIC)){\
        m6_log_error("Could not append new"long_name"option to options list\n");\
        return -1;\
    }\
    return result;\
}


//Declare all the options parsers
m6_options_add_declare(M6_BOOL,     m6_bool,    b, "boolean");
m6_options_add_declare(M6_UINT64,   u64,        u, "unsigned");
m6_options_add_declare(M6_INT64,    i64,        i, "integer");
m6_options_add_declare(M6_STRING,   char*,      s, "string");
m6_options_add_declare(M6_DOUBLE,   double,     f, "float");


#define USE_M6_OPTIONS \
    m6_options_t opts = {0};

int m6_options_name(char* description);
int m6_options_tail(char* description);
int m6_options_short_description(char* description);
int m6_options_long_description(char* description);
int m6_options_parse(int argc, char** argv);


#endif /* M6_OPTIONS_H_ */
