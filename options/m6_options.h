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
    int help;
    int unlimted_set;
} m6_options_t;


int m6_options_addb(m6_options_mode_e mode, char short_str, const char* long_str, const char* descr, const m6_bool def);
int m6_options_addi(m6_options_mode_e mode, char short_str, const char* long_str, const char* descr, const i64 def);
int m6_options_addu(m6_options_mode_e mode, char short_str, const char* long_str, const char* descr, const u64 def);
int m6_options_addf(m6_options_mode_e mode, char short_str, const char* long_str, const char* descr, const double def);
int m6_options_adds(m6_options_mode_e mode, char short_str, const char* long_str, const char* descr, const char*);


int m6_options_init();

#define USE_M6_OPTIONS \
    m6_options_t opts = {0};\
    m6_options_init()


int m6_options_name(char* description);
int m6_options_tail(char* description);
int m6_options_short_description(char* description);
int m6_options_long_description(char* description);
int m6_options_add(m6_options_mode_e mode, const char short_str, const char* long_str, const char* descr, m6_types_e type, ...);
int m6_options_parse(int argc, char** argv);


#endif /* M6_OPTIONS_H_ */
