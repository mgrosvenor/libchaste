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
    char* long_str;
    char* descr;
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


//Declare all the options parsers for non vector types, with initializers
#define m6_opt_add_declare_i(m6_type_name, c_type_name, short_name, long_name)\
inline int m6_opt_add##short_name##i(m6_options_mode_e mode, char short_str, char* long_str, char* descr, c_type_name* result_out, c_type_name default_val)
m6_opt_add_declare_i(M6_BOOL,     m6_bool,    b, "boolean");
m6_opt_add_declare_i(M6_UINT64,   u64,        u, "unsigned");
m6_opt_add_declare_i(M6_INT64,    i64,        i, "integer");
m6_opt_add_declare_i(M6_STRING,   char*,      s, "string");
m6_opt_add_declare_i(M6_DOUBLE,   double,     f, "float");

//Declare all the options parsers for non vector types, untantalized
#define m6_opt_add_declare_u(m6_type_name, c_type_name, short_name, long_name)\
inline int m6_opt_add##short_name##u(m6_options_mode_e mode, char short_str, char* long_str, char* descr, c_type_name* result_out)
m6_opt_add_declare_u(M6_BOOL,     m6_bool,    b, "boolean");
m6_opt_add_declare_u(M6_UINT64,   u64,        u, "unsigned");
m6_opt_add_declare_u(M6_INT64,    i64,        i, "integer");
m6_opt_add_declare_u(M6_STRING,   char*,      s, "string");
m6_opt_add_declare_u(M6_DOUBLE,   double,     f, "float");

//Declare all the options parsers for vector types, with initializers
#define m6_opt_add_declare_VI(m6_type_name, c_type_name_default, short_name, long_name)\
inline int m6_opt_add##short_name##I(m6_options_mode_e mode, char short_str, char* long_str, char* descr, Vector* result_out, c_type_name_default default_val)
m6_opt_add_declare_VI(M6_BOOLS,    m6_bool,    B, "booleans");
m6_opt_add_declare_VI(M6_UINT64S,  u64,        U, "unsigneds");
m6_opt_add_declare_VI(M6_INT64S,   i64,        I, "integers");
m6_opt_add_declare_VI(M6_STRINGS,  char*,      S, "strings");
m6_opt_add_declare_VI(M6_DOUBLES,  double,     F, "floats");

//Declare all the options parsers for vector types, with uninitialized
#define m6_opt_add_declare_VU(m6_type_name, c_type_name, short_name, long_name)\
inline int m6_opt_add##short_name##U(m6_options_mode_e mode, char short_str, char* long_str, char* descr, Vector* result_out)
m6_opt_add_declare_VU(M6_BOOLS,    m6_bool,    B, "booleans");
m6_opt_add_declare_VU(M6_UINT64S,  u64,        U, "unsigneds");
m6_opt_add_declare_VU(M6_INT64S,   i64,        I, "integers");
m6_opt_add_declare_VU(M6_STRINGS,  char*,      S, "strings");
m6_opt_add_declare_VU(M6_DOUBLES,  double,     F, "floats");


#define USE_M6_OPTIONS \
    m6_options_t opts = {0};

int m6_opt_name(char* description);
int m6_opt_tail(char* description);
int m6_opt_short_description(char* description);
int m6_opt_long_description(char* description);
int m6_opt_parse(int argc, char** argv);


#endif /* M6_OPTIONS_H_ */
