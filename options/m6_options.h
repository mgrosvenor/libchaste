/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Tools to parse and print command line options
 */

#ifndef CAMIO_OPTIONS_H_
#define CAMIO_OPTIONS_H_

#include "../types/camio_types.h"

typedef enum {
    CAMIO_OPTION_REQUIRED,       //This option is required for the program to run
    CAMIO_OPTION_OPTIONAL,       //This option is not required but has a default value
    CAMIO_OPTION_UNLIMTED,       //This option produces a list of outputs
    CAMIO_OPTION_FLAG,           //This option is optional, and has no arguments
} camio_options_mode_e;



typedef struct camio_options_opt{
    camio_options_mode_e mode;
    camio_types_e type;
    void* var;
    char short_str;
    const char* long_str;
    const char* descr;
    int found;
    struct camio_options_opt* next;
} camio_options_opt_t;


typedef struct {
    char* short_description;
    char* long_description;
    char* head;
    char* tail;
    camio_options_opt_t* opt_def;
    uint64_t count;
    int help;
    int unlimted_set;
} camio_options_t;


camio_options_t* fep2_options_new();
int camio_options_name(char* description);
int camio_options_tail(char* description);
int camio_options_short_description(char* description);
int camio_options_long_description(char* description);
int camio_options_add(camio_options_mode_e mode, const char short_str, const char* long_str, const char* descr, camio_types_e type, ...);
int camio_options_parse(int argc, char** argv);


#endif /* CAMIO_OPTIONS_H_ */
