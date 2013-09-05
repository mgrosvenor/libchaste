/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Tools to parse and print command line options
 */



#include <stdarg.h>
#include <getopt.h>
#include <libio.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../types/types.h"
#include "options.h"

#include "../utils/util.h"
#include "../log/log.h"

#include "../parsing/numeric_parser.h"
#include "../parsing/bool_parser.h"


extern ch_options_t opts;
void ch_options_init()
{
    construct(Vector,&opts.opt_defs,sizeof(ch_options_opt_t),FREEOBJ);
    opts.done_init = 1;
}



void print_usage(const char* err_tx_fmt, ...){

    if(opts.short_description)
        printf("\n%s:\n\n", opts.short_description);


    for (u64 i = 0; i < size(Vector,&opts.opt_defs); i++ ) {
        ch_options_opt_t* opt_def = ((ch_options_opt_t*)opts.opt_defs.mem) + i;

        char* mode = NULL;
        switch(opt_def->mode){
            case CH_OPTION_FLAG:      mode = "Flag"; break;
            case CH_OPTION_REQUIRED:  mode = "Required"; break;
            case CH_OPTION_OPTIONAL:  mode = "Optional"; break;
            case CH_OPTION_UNLIMTED:  mode = "Unlimited"; break;
        }

        char* type = NULL;
        switch(opt_def->type){
            case CH_BOOL:       type = "boolean";   break;
            case CH_INT64:      type = "integer";   break;
            case CH_UINT64:     type = "unsigned";  break;
            case CH_DOUBLE:     type = "float";     break;
            case CH_STRING:     type = "string";    break;
            case CH_BOOLS:      type = "booleans";  break;
            case CH_INT64S:     type = "integers";  break;
            case CH_UINT64S:    type = "unsigneds"; break;
            case CH_DOUBLES:    type = "floats";    break;
            case CH_STRINGS:    type = "strings";   break;
            case CH_NO_TYPE:    type = "error";     break;
        }


        printf("%-9s (%-9s) -%c  --%-15s - %s\n", mode, type, opt_def->short_str, opt_def->long_str,  opt_def->descr);
    }

    if(opts.long_description)
        printf("%s\n\n", opts.long_description);

    if(err_tx_fmt){
        va_list args;
        va_start(args, err_tx_fmt);
        ch_log_error_va(err_tx_fmt,args);
        va_end(args);
        exit(-1);
    }

    exit(0);
}



int ch_options_tail(char* description){
    (void)description;
    ch_log_fatal("Function unimplemented");
    return 0;
}

int ch_options_short_description(char* description){
    opts.short_description = description;
    return 0;
}

int ch_options_long_description(char* description){
    opts.long_description= description;
    return 0;
}


//Generic options addition code
static ch_word ch_options_add_generic(
        ch_options_opt_t* opt_def_new,
        ch_options_mode_e mode,
        char short_str,
        char* long_str,
        char* descr,
        ch_types_e type,
        void* result_out)
{

    if(!opts.done_init){
        ch_options_init();
    }


    if(mode == CH_OPTION_FLAG && type != CH_BOOL){
        ch_log_error( "Flag options must be used with boleans only\n");
        return -1;
    }

    if(mode == CH_OPTION_UNLIMTED){
        if(!opts.unlimted_set){
            if(is_vector(type)){
                opts.unlimted_set = 1;
            }
            else{
                ch_log_error( "Unlimited options must be used with a list type variable eg ch_list(string)\n");
                return -1;
            }
        }
        else{
            ch_log_error( "Unlimited options can only be used once!\n");
            return -1;
        }
    }

    //Create a new opt def
    bzero(opt_def_new,sizeof(ch_options_opt_t));

    opts.count++;
    opt_def_new->mode      = mode;
    opt_def_new->short_str = short_str;
    opt_def_new->long_str  = long_str;
    opt_def_new->descr     = descr;
    opt_def_new->type      = type;
    opt_def_new->var       = result_out;


    for( u64 i = 0; i < size(Vector,&opts.opt_defs); i++){
        ch_options_opt_t* opt_def = ((ch_options_opt_t*)opts.opt_defs.mem) + i;
        if(opt_def->short_str == opt_def_new->short_str) {

            //Special case 'h' for help as we want it to stick around
            //and its added implicitly so devs will get confused
            const char* long_str = opt_def->short_str != 'h' ? opt_def_new->long_str : opt_def->long_str;
            ch_log_fatal("Could not add option. \"--%s\". The short name \"-%c\" conflicts with an existing option's short name \"-%c\" \n",
                    long_str,
                    opt_def_new->short_str,
                    opt_def->short_str);

            return -1;
        }
        if(!strcmp(opt_def->long_str, opt_def_new->long_str)) {
            const char short_str = strcmp("help",opt_def->long_str) ? opt_def_new->short_str : opt_def->short_str;
            ch_log_fatal("Could not add option. \"-%c\" . The long name \"--%s\" conflicts with an existing option's long name \"--%s\" \n",
                    short_str,
                    opt_def_new->long_str,
                    opt_def->long_str);
            return -1;
        }

    }

    return 0;

}

//##########################################################################################################################
//Define all the options parsers for non vector types, with initializers
#define ch_opt_add_define_i(ch_type_name, c_type_name, short_name, long_name)\
ch_opt_add_declare_i(ch_type_name, c_type_name, short_name, long_name)\
{\
    ch_options_opt_t opt_new = (const struct ch_options_opt){0};\
    *result_out = default_val;\
    \
    ch_word result = ch_options_add_generic(&opt_new, mode, short_str, long_str, descr, ch_type_name, result_out);\
    if(result){ /*Catch errors inside add generic, it prints it's own message*/\
        return result;\
    }\
    \
    if(push_back(Vector,&opts.opt_defs, &opt_new,STATIC)){\
        ch_log_error("Could not append new"long_name"option to options list\n");\
        return -1;\
    }\
    \
    return result;\
}

ch_opt_add_define_i(CH_BOOL,     ch_bool,    b, "boolean")
ch_opt_add_define_i(CH_UINT64,   u64,        u, "unsigned")
ch_opt_add_define_i(CH_INT64,    i64,        i, "integer")
ch_opt_add_define_i(CH_STRING,   char*,      s, "string")
ch_opt_add_define_i(CH_DOUBLE,   double,     f, "float")
//##########################################################################################################################

//Define all the options parsers for non vector types, without initializers
#define ch_opt_add_define_u(ch_type_name, c_type_name, short_name, long_name)\
ch_opt_add_declare_u(ch_type_name, c_type_name, short_name, long_name)\
{\
    if(mode == CH_OPTION_OPTIONAL || mode == CH_OPTION_FLAG){\
        ch_log_fatal("CH options marked as \"optional\" or \"flag\" must have an initializer value. Use \"ch_opt_add_"#short_name"i()\" instead.\n");\
        return -1; \
    }\
    \
    ch_options_opt_t opt_new = {0};\
    \
    ch_word result = ch_options_add_generic(&opt_new, mode, short_str, long_str, descr, ch_type_name, result_out);\
    if(result){ /*Catch errors inside add generic, it prints it's own message*/\
        return result;\
    }\
    \
    if(push_back(Vector,&opts.opt_defs, &opt_new,STATIC)){\
        ch_log_error("Could not append new"long_name"option to options list\n");\
        return -1;\
    }\
    \
    return result;\
}
ch_opt_add_define_u(CH_BOOL,     ch_bool,    b, "boolean")
ch_opt_add_define_u(CH_UINT64,   u64,        u, "unsigned")
ch_opt_add_define_u(CH_INT64,    i64,        i, "integer")
ch_opt_add_define_u(CH_STRING,   char*,      s, "string")
ch_opt_add_define_u(CH_DOUBLE,   double,     f, "float")
//##########################################################################################################################

//Define all the options parsers for non vector types, with initializers
#define ch_opt_add_define_VI(ch_type_name, c_type_name_default, short_name, long_name)\
ch_opt_add_declare_VI(ch_type_name, c_type_name_default, short_name, long_name)\
{\
    ch_options_opt_t opt_new = {0};\
    memset(result_out, 0, sizeof(Vector));\
    construct(Vector, result_out, sizeof(c_type_name_default), FREEOBJ);\
    push_back(Vector,result_out,&default_val,STATIC);\
    \
    ch_word result = ch_options_add_generic(&opt_new, mode, short_str, long_str, descr, ch_type_name, result_out);\
    if(result){ /*Catch errors inside add generic, it prints it's own message*/\
        return result;\
    }\
    if(push_back(Vector,&opts.opt_defs, &opt_new,STATIC)){\
        ch_log_error("Could not append new vector of "long_name" option to options list\n");\
        return -1;\
    }\
    \
    return result;\
}
ch_opt_add_define_VI(CH_BOOLS,    ch_bool,    B, "booleans")
ch_opt_add_define_VI(CH_UINT64S,  u64,        U, "unsigneds")
ch_opt_add_define_VI(CH_INT64S,   i64,        I, "integers")
ch_opt_add_define_VI(CH_STRINGS,  char*,      S, "strings")
ch_opt_add_define_VI(CH_DOUBLES,  double,     F, "floats")
//##########################################################################################################################

//Define all the options parsers for non vector types, without initializers
#define ch_opt_add_define_VU(ch_type_name, c_type_name, short_name, long_name)\
ch_opt_add_declare_VU(ch_type_name, c_type_name, short_name, long_name)\
{\
    if(mode == CH_OPTION_OPTIONAL){\
        ch_log_fatal("CH vector options marked as \"optional\" must have an initializer value. Use \"ch_opt_add_"#short_name"U()\" instead.\n");\
        return -1; \
    }\
    \
    ch_options_opt_t opt_new = {0};\
    memset(result_out, 0, sizeof(Vector));\
    construct(Vector, result_out, sizeof(c_type_name), FREEOBJ);\
    \
    ch_word result = ch_options_add_generic(&opt_new, mode, short_str, long_str, descr, ch_type_name, result_out);\
    if(result){ /*Catch errors inside add generic, it prints it's own message*/\
        return result;\
    }\
    if(push_back(Vector,&opts.opt_defs, &opt_new,STATIC)){\
        ch_log_error("Could not append new vector of "long_name" option to options list\n");\
        return -1;\
    }\
    \
    return result;\
}
ch_opt_add_define_VU(CH_BOOLS,    ch_bool,    B, "booleans")
ch_opt_add_define_VU(CH_UINT64S,  u64,        U, "unsigneds")
ch_opt_add_define_VU(CH_INT64S,   i64,        I, "integers")
ch_opt_add_define_VU(CH_STRINGS,  char*,      S, "strings")
ch_opt_add_define_VU(CH_DOUBLES,  double,     F, "floats")
//##########################################################################################################################


void parse_argument(ch_options_opt_t* opt_def) {

    if(is_vector(opt_def->type)){
        if(opt_def->found == 1 && size(Vector,(Vector*)opt_def->var) == 1 ){ //This is the first option found
            pop_back(Vector,(Vector*)opt_def->var); //Remove the default value
        }
    }


    num_result_t num_result;
    switch (opt_def->type) {
        case CH_INT64:
        case CH_INT64S: {
            //Sanity check
            if (!optarg) { print_usage( "Option --%s (-%c) Expected argument of type INT64 but none found.\n", opt_def->long_str, opt_def->short_str); }

            //Get the number
            num_result = parse_number(optarg, 0);
            if(num_result.type == CH_UINT64){ num_result.type = CH_INT64;  } //Type promote uint to int
            if (num_result.type != CH_INT64) { print_usage( "Option --%s (-%c) Expected argument of type INT64 but \"%s\" found\n", opt_def->long_str, opt_def->short_str, optarg);}

            //Assign it
            if (opt_def->type == CH_INT64) {
                *((int64_t*)opt_def->var) = num_result.val_int;
            }
            else {
                push_back(Vector,opt_def->var, &num_result.val_int, STATIC);
            }
            break;
        }

        case CH_UINT64:
        case CH_UINT64S: {
            //Sanity check
            if (!optarg) { print_usage( "Option  --%s (-%c) Expected argument of type UINT64 but none found.\n", opt_def->long_str, opt_def->short_str); }

            //Get the number
            num_result = parse_number(optarg, 0);
            if (num_result.type != CH_UINT64) { print_usage("Option  --%s (-%c) Expected argument of type UINT64 but \"%s\" found\n", opt_def->long_str, opt_def->short_str, optarg);}

            //Assign it
            if (opt_def->type == CH_UINT64){
                *((uint64_t*)opt_def->var) = num_result.val_uint;
            }
            else{
                push_back(Vector,opt_def->var, &num_result.val_uint, STATIC);
            }
            break;
        }

        case CH_DOUBLE:
        case CH_DOUBLES: {
            //Sanity check
            if (!optarg) { print_usage("Option --%s (-%c) Expected argument of type DOUBLE but none found.\n", opt_def->long_str, opt_def->short_str);}

            //Get the number
            num_result = parse_number(optarg, 0);
            double result = 0;
            if(num_result.type == CH_DOUBLE){
                result = num_result.val_dble;
            }
            //Type promote int to double
            else if(num_result.type == CH_UINT64){
                result = (double)num_result.val_int;
            }
            //Type promote uint to double
            else if(num_result.type == CH_UINT64){
                result = (double)num_result.val_uint;
            }
            //No Suitable type found
            else{
                print_usage("Option --%s (-%c) Expected argument of type DOUBLE but \"%s\" found\n", opt_def->long_str, opt_def->short_str, optarg);
            }

            //Assign it
            if (opt_def->type == CH_DOUBLE){
                *(double*) opt_def->var = result;
            }
            else{
                push_back(Vector,opt_def->var, &result, STATIC);
            }

            break;
        }

        case CH_BOOL:
        case CH_BOOLS: {
            //Sanity check
            if (!optarg) { print_usage("Option --%s (-%c) Expected argument of type BOOL but none found.\n", opt_def->long_str, opt_def->short_str);}

            //Get the number
            num_result = parse_bool(optarg, strlen(optarg), 0);
            if (num_result.type != CH_INT64) { print_usage("Option --%s (-%c) Expected argument of type BOOL but \"%s\" found\n", opt_def->long_str, opt_def->short_str,  optarg);}

            //Assign it
            if (opt_def->type == CH_BOOL){
                *((int*)opt_def->var) = (int) num_result.val_int;
            }
            else{
                push_back(Vector,opt_def->var, &num_result.val_int, STATIC);
            }

            break;
        }

        case CH_STRING:
        case CH_STRINGS: {
            //Sanity check
            if (!optarg) { print_usage("Option --%s (-%c) Expected argument of type STRING but none found.\n", opt_def->long_str, opt_def->short_str);}

            //Assign it
            if (opt_def->type == CH_STRING) {
                * ((char**)opt_def->var) = optarg;
            }
            else{
                push_back(Vector,opt_def->var, &optarg, STATIC);
            }

            break;
        }

        default: {
            print_usage("Option --%s (-%c) argument has unknown type (%s)\n", opt_def->long_str, opt_def->short_str, optarg);
            break;
        }
    }
}

//Search through the options, find the option with the matching character
void process_option(char c) {

    //int done = 0;
    ch_options_opt_t* opt_def = NULL;
    for ( u64 i = 0; i < size(Vector,&opts.opt_defs) ; i++) {
        opt_def = ((ch_options_opt_t*)opts.opt_defs.mem) + i;

        if (opt_def->short_str == c) {
            //done = 1; //Exit the loop when finished

            opt_def->found++; //We found an option of this type
            if (opt_def->found > 1 && !is_vector(opt_def->type)) {
                print_usage("Option --%s (-%c), only one instance expected but multiple found.\n", opt_def->long_str, opt_def->short_str);
            }

            if (opt_def->mode == CH_OPTION_FLAG) {
                *(int*)opt_def->var = ! (*(int*)opt_def->var); //invert the default value
                continue;  //This is all handled by getopts
            }

            //Parse and assign the argument
            parse_argument(opt_def);
        }
    }
}


//Fill in the getopts and getopts_long structures
void generate_unix_opts(char short_opts_str[1024], struct option* long_options) {
    size_t i = 0;

    char* short_opts_ptr = short_opts_str;

    VectorIter* iter = create(VectorIter, &opts.opt_defs);
    ch_bool has_next = true;
    for (head(VectorIter,iter); has_next && !empty(Vector,&opts.opt_defs) && short_opts_ptr < &short_opts_str[1024]; (has_next = !next(VectorIter,iter)), i++ ) {
        ch_options_opt_t* opt_def = (ch_options_opt_t*)retrieve(VectorIter,iter);

        *short_opts_ptr = opt_def->short_str;
        short_opts_ptr++;

        long_options[i].name = opt_def->long_str;

        if (opt_def->mode == CH_OPTION_FLAG) {
            long_options[i].name = opt_def->long_str;
            long_options[i].has_arg = no_argument;
            long_options[i].flag = opt_def->var;
            long_options[i].val = !(*(int*) opt_def->var);
        } else {
            long_options[i].name = opt_def->long_str;
            long_options[i].has_arg = required_argument;
            long_options[i].flag = NULL;
            long_options[i].val = opt_def->short_str;
            *short_opts_ptr = ':';
            short_opts_ptr++;
        }


    }
    long_options[i].flag = NULL;
    long_options[i].has_arg = 0;
    long_options[i].name = NULL;
    long_options[i].val = 0;
}

int ch_opt_parse(int argc, char** argv){
    ch_opt_addbi(CH_OPTION_FLAG, 'h', "help", "Print this help message\n", &opts.help, 0);

    char short_opts_str[1024] = {0};

    //Disable internal error reporting
    opterr = 0;

    //Form the getopts string
    struct option* long_options = (struct option*)malloc((opts.count + 1) * sizeof(struct option) );
    bzero(long_options, (opts.count + 1) * sizeof(struct option));

    generate_unix_opts(short_opts_str, long_options);

    int option_index = 0;
    char c = getopt_long (argc, argv, short_opts_str, long_options, &option_index);
    for(; c != EOF; c = getopt_long(argc, argv, short_opts_str, long_options, &option_index)){

        if( c == 'h' || opts.help){
            print_usage(NULL);
            exit(0);
        }

        //Search through the options, find the option with the matching character
        if(c == '?'){
            print_usage("Unknown option or option is missing an argument \"%s\"\n", argv[optind -1]);
        }

        process_option(c);

    }

    if (optind < argc){ //There are extra parameters
        //Look for an opt_def with type UNLIMITED
        ch_options_opt_t* opt_def = NULL;
        u64 i = 0;
        for (; i < size(Vector,&opts.opt_defs) ; i++) {
            opt_def = ((ch_options_opt_t*)opts.opt_defs.mem) + i;
            if(opt_def->mode == CH_OPTION_UNLIMTED){
                break;
            }
        }

        //None found
        if(i >= size(Vector,&opts.opt_defs)){
            print_usage("Unknown option \"%s\"\n", argv[optind]);
        }

        //Parse the remaning options
        for(; optind < argc; optind++){
            optarg = argv[optind];
            opt_def->found++;
            parse_argument(opt_def);
        }
    }


    //Check the constraints
    ch_options_opt_t* opt_def = NULL;
    for (u64 i = 0; i < size(Vector,&opts.opt_defs) ; i++) {
        opt_def = ((ch_options_opt_t*)opts.opt_defs.mem) + i;
        if(opt_def->mode == CH_OPTION_REQUIRED && opt_def->found < 1){
            printf("Option --%s (-%c) is required but not supplied\n", opt_def->long_str, opt_def->short_str);
            print_usage("Option --%s (-%c) is required but not supplied\n", opt_def->long_str, opt_def->short_str);
        }
    }


    return 0;
}
