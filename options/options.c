/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Tools to parse and print command line options
 */



#include <stdarg.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../types/types.h"
#include "options.h"

#include "../utils/util.h"
#include "../log/log.h"

#include "../parsing/numeric_parser.h"
#include "../parsing/bool_parser.h"
#include "../data_structs/vector/vector_std.h"


int cmp(ch_options_opt_t* lhs, ch_options_opt_t* rhs)
{
    return lhs->short_opt == rhs->short_opt ? 0 : lhs->short_opt < rhs->short_opt ? -1 : 1 ;
}


extern ch_options_t opts;

void ch_options_init()
{
    opts.opt_defs = CH_VECTOR_NEW(opts,256,cmp);
    opts.done_init = 1;
}


void ch_opt_print_usage(const char* err_tx_fmt, ...){

    FILE* outfd = err_tx_fmt ? stderr : stdout;

    if(!opts.noprint_short && !err_tx_fmt && opts.short_description){
        fprintf(outfd, "%s\n", opts.short_description);
    }

    //opts.opt_defs->sort(opts.opt_defs);


    for (ch_options_opt_t* opt_def = opts.opt_defs->first;
            opt_def < opts.opt_defs->end;
            opt_def = opts.opt_defs->next(opts.opt_defs, opt_def) ) {

        if(opts.print_mode){
            char* mode = NULL;
            switch(opt_def->mode){
                case CH_OPTION_FLAG:      mode = "Flag"; break;
                case CH_OPTION_REQUIRED:  mode = "Required"; break;
                case CH_OPTION_OPTIONAL:  mode = "Optional"; break;
                case CH_OPTION_UNLIMTED:  mode = "Unlimited"; break;
                case CH_OPTION_MANUAL:    mode = "Manual"; break;
            }
            fprintf(outfd, "%-9s ", mode);

        }

        if(opts.print_type){
            char* type = NULL;
            switch(opt_def->type){
                case CH_BOOL:       type = "Boolean";   break;
                case CH_INT64:      type = "Integer";   break;
                case CH_UINT64:     type = "Unsigned";  break;
                case CH_DOUBLE:     type = "Float";     break;
                case CH_STRING:     type = "String";    break;
                case CH_HEX:        type = "Unsigned";  break;
                case CH_BOOLS:      type = "Boolean List";  break;
                case CH_INT64S:     type = "Integers List"; break;
                case CH_UINT64S:    type = "Unsigned List"; break;
                case CH_DOUBLES:    type = "Float List";    break;
                case CH_STRINGS:    type = "String List";   break;
                case CH_HEXS:       type = "Unsigned List"; break;
                case CH_NO_TYPE:    type = "error";     break;
            }
            fprintf(outfd, "(%-11s) ", type);
        }

        const char* short_opt = opt_def->short_opt ? "-" : "  ";
        fprintf(outfd, "%s%c  --%*s  %*s  ", short_opt, opt_def->short_opt,
                -opts.max_long_opt_len, opt_def->long_opt,
                -opts.max_descr_len, opt_def->descr);


        if(!opts.noprint_defualt && opt_def->has_default &&
                (opt_def->mode == CH_OPTION_OPTIONAL || opt_def->mode == CH_OPTION_MANUAL)){
            switch(opt_def->type){
                case CH_BOOL:       fprintf(outfd,"[%s]",  *(ch_bool*)opt_def->var ? "True" : "False");   break;
                case CH_INT64:      fprintf(outfd,"[%lli]", *(ch_word*)opt_def->var);   break;
                case CH_UINT64:     fprintf(outfd,"[%llu]", *(u64*)opt_def->var);   break;
                case CH_DOUBLE:     fprintf(outfd,"[%lf]", *(double*)opt_def->var);   break;
                case CH_STRING:     fprintf(outfd,"[%s]",  *(char**)opt_def->var);   break;
                case CH_HEX:        fprintf(outfd,"[0x%016llX]", *(u64*)opt_def->var);  break;
                case CH_BOOLS:      fprintf(outfd,"[%s]",  ( *((CH_VECTOR(ch_bool)*)opt_def->var)->first) ? "True" : "False"); break;
                case CH_INT64S:     fprintf(outfd,"[%lli]", *((CH_VECTOR(word)*)opt_def->var)->first );  break;
                case CH_UINT64S:    fprintf(outfd,"[%llu]", *((CH_VECTOR(u64)*)opt_def->var)->first );  break;
                case CH_DOUBLES:    fprintf(outfd,"[%lf]", *((CH_VECTOR(float)*)opt_def->var)->first );  break;
                case CH_STRINGS:    fprintf(outfd,"[%s]", *((CH_VECTOR(cstr)*)opt_def->var)->first );  break;
                case CH_HEXS:       fprintf(outfd,"[0x%016llX]", *((CH_VECTOR(u64)*)opt_def->var)->first );  break;
                case CH_NO_TYPE:    fprintf(outfd, "[Unknown]");  break;
            }

        }

        fprintf(outfd,"\n");
    }

    if(!opts.noprint_long && !err_tx_fmt && opts.long_description)
        fprintf(outfd,"\n%s", opts.long_description);

    if(err_tx_fmt){
        va_list args;
        va_start(args, err_tx_fmt);
        fprintf(stderr,"\n");
        vfprintf(stderr, err_tx_fmt,args);
        va_end(args);
        exit(-1);
    }

    if(!opts.noexit)
        exit(0);
}



int ch_options_tail(char* description){
    (void)description;
    ch_log_fatal("Function unimplemented");
    return 0;
}

int ch_opt_short_description(char* description){
    opts.short_description = description;
    return 0;
}

int ch_opt_long_description(char* description){
    opts.long_description = description;
    return 0;
}


//Generic options addition code
static ch_word ch_options_add_generic(
        ch_options_opt_t* opt_def_new,
        ch_options_mode_e mode,
        char short_opt,
        char* long_opt,
        char* descr,
        ch_types_e type,
        void* result_out,
        int has_default)
{

    if(!opts.done_init){
        ch_options_init();
    }


    if(mode == CH_OPTION_FLAG && type != CH_BOOL){
        ch_log_error( "Flag options must be used with booleans only\n");
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

    opts.max_long_opt_len = MAX(opts.max_long_opt_len, strlen(long_opt));
    opts.max_descr_len = MAX(opts.max_descr_len, strlen(descr));
    opts.count++;
    opt_def_new->mode        = mode;
    opt_def_new->short_opt   = short_opt;
    opt_def_new->long_opt    = long_opt;
    opt_def_new->descr       = descr;
    opt_def_new->type        = type;
    opt_def_new->var         = result_out;
    opt_def_new->has_default = has_default;
    //printf("New opt: %c = %p\n", opt_def_new->short_opt, opt_def_new->var);

    //Make sure the option hasn't already been defined
    for (ch_options_opt_t* opt_def = opts.opt_defs->first;
            opt_def < opts.opt_defs->end;
            opt_def = opts.opt_defs->next(opts.opt_defs, opt_def) ) {

        if(opt_def->short_opt != '\0' && opt_def->short_opt == opt_def_new->short_opt) {

            //Special case 'h' for help as we want it to stick around
            //and its added implicitly so devs will get confused
            const char* long_opt = opt_def->short_opt != 'h' ? opt_def_new->long_opt : opt_def->long_opt;
            ch_log_fatal("Could not add option. \"--%s\". The short name \"-%c\" conflicts with an existing option's short name \"-%c\" \n",
                    long_opt,
                    opt_def_new->short_opt,
                    opt_def->short_opt);

            return -1;
        }
        if(!strcmp(opt_def->long_opt, opt_def_new->long_opt)) {
            const char short_opt = strcmp("help",opt_def->long_opt) ? opt_def_new->short_opt : opt_def->short_opt;
            ch_log_fatal("Could not add option. \"-%c\" . The long name \"--%s\" conflicts with an existing option's long name \"--%s\" \n",
                    short_opt,
                    opt_def_new->long_opt,
                    opt_def->long_opt);
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
    ch_word result = ch_options_add_generic(&opt_new, mode, short_opt, long_opt, descr, ch_type_name, result_out, true);\
    if(result){ /*Catch errors inside add generic, it prints it's own message*/\
        return result;\
    }\
    \
    if(!opts.opt_defs->push_back(opts.opt_defs,opt_new)){\
        ch_log_error("Could not append new"long_name"option to options list\n");\
        return -1;\
    }\
    \
    return result;\
}

ch_opt_add_define_i(CH_BOOL,     ch_bool,    b, "Boolean")
ch_opt_add_define_i(CH_UINT64,   u64,        u, "Unsigned")
ch_opt_add_define_i(CH_INT64,    ch_word,    i, "Integer")
ch_opt_add_define_i(CH_STRING,   ch_cstr,    s, "String")
ch_opt_add_define_i(CH_DOUBLE,   ch_float,   f, "Float")
ch_opt_add_define_i(CH_HEX,      u64,        x, "Unsigned")
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
    ch_word result = ch_options_add_generic(&opt_new, mode, short_opt, long_opt, descr, ch_type_name, result_out, false);\
    if(result){ /*Catch errors inside add generic, it prints it's own message*/\
        return result;\
    }\
    \
    if(!opts.opt_defs->push_back(opts.opt_defs, opt_new)){\
        ch_log_error("Could not append new"long_name"option to options list\n");\
        return -1;\
    }\
    \
    return result;\
}
ch_opt_add_define_u(CH_BOOL,     ch_bool,    b, "Boolean")
ch_opt_add_define_u(CH_UINT64,   u64,        u, "Unsigned")
ch_opt_add_define_u(CH_INT64,    ch_word,    i, "Integer")
ch_opt_add_define_u(CH_STRING,   ch_cstr,    s, "String")
ch_opt_add_define_u(CH_DOUBLE,   ch_float,   f, "Float")
ch_opt_add_define_u(CH_HEX,      u64,        x, "Unsigned")
//##########################################################################################################################

//Define all the options parsers for vector types, with initializers
#define ch_opt_add_define_VI(ch_type_name, vector_name, c_type_name_default, short_name, long_name)\
ch_opt_add_declare_VI(ch_type_name, vector_name, c_type_name_default, short_name, long_name)\
{\
    ch_options_opt_t opt_new = {0};\
    *result_out = CH_VECTOR_NEW(vector_name, 4, CH_VECTOR_CMP(vector_name));\
    (*result_out)->push_back(*result_out, default_val);\
    \
    ch_word result = ch_options_add_generic(&opt_new, mode, short_opt, long_opt, descr, ch_type_name, *result_out, true);\
    if(result){ /*Catch errors inside add generic, it prints it's own message*/\
        return result;\
    }\
    if(!opts.opt_defs->push_back(opts.opt_defs, opt_new)){\
        ch_log_error("Could not append new vector of "long_name" option to options list\n");\
        return -1;\
    }\
    \
    return result;\
}

ch_opt_add_define_VI(CH_BOOLS,    ch_bool, ch_bool,    B, "Boolean List")
ch_opt_add_define_VI(CH_UINT64S,  u64,     u64,        U, "Unsigned List")
ch_opt_add_define_VI(CH_INT64S,   word,    ch_word,    I, "Integer List")
ch_opt_add_define_VI(CH_STRINGS,  cstr,    ch_cstr,    S, "String List")
ch_opt_add_define_VI(CH_DOUBLES,  float,   ch_float,   F, "Float List")
ch_opt_add_define_VI(CH_HEXS,     u64,     u64,        X, "Unsigned List")
//##########################################################################################################################

//Define all the options parsers for vector types, without initializers
#define ch_opt_add_define_VU(ch_type_name, vector_name, short_name, long_name)\
ch_opt_add_declare_VU(ch_type_name, vector_name, short_name, long_name)\
{\
    if(mode == CH_OPTION_OPTIONAL){\
        ch_log_fatal("CH vector options marked as \"optional\" must have an initializer value. Use \"ch_opt_add_"#short_name"U()\" instead.\n");\
        return -1; \
    }\
    \
    ch_options_opt_t opt_new = {0};\
    *result_out = CH_VECTOR_NEW(vector_name, 4, CH_VECTOR_CMP(vector_name));\
    \
    ch_word result = ch_options_add_generic(&opt_new, mode, short_opt, long_opt, descr, ch_type_name, *result_out, false);\
    if(result){ /*Catch errors inside add generic, it prints it's own message*/\
        return result;\
    }\
    if(!opts.opt_defs->push_back(opts.opt_defs, opt_new)){\
        ch_log_error("Could not append new vector of "long_name" option to options list\n");\
        return -1;\
    }\
    \
    return result;\
}

ch_opt_add_define_VU(CH_BOOLS,    ch_bool,    B, "Booleans List")
ch_opt_add_define_VU(CH_UINT64S,  u64,        U, "Unsigneds List")
ch_opt_add_define_VU(CH_INT64S,   word,       I, "Integers List")
ch_opt_add_define_VU(CH_STRINGS,  cstr,       S, "Strings List")
ch_opt_add_define_VU(CH_DOUBLES,  float,      F, "Floats List")
ch_opt_add_define_VU(CH_HEXS,     u64,        X, "Unsigned List")
//##########################################################################################################################


void parse_argument(ch_options_opt_t* opt_def) {

    num_result_t num_result;
    switch (opt_def->type) {
        case CH_INT64:
        case CH_INT64S: {
            //Sanity check
            if (!optarg) { ch_opt_print_usage( "Option --%s (-%c) Expected argument of type INT64 but none found.\n", opt_def->long_opt, opt_def->short_opt); }

            //Get the number
            num_result = parse_number(optarg, 0);
            if(num_result.type == CH_UINT64){ num_result.type = CH_INT64;  } //Type promote uint to int
            if (num_result.type != CH_INT64) { ch_opt_print_usage( "Option --%s (-%c) Expected argument of type INT64 but \"%s\" found\n", opt_def->long_opt, opt_def->short_opt, optarg);}

            //Assign it
            if (opt_def->type == CH_INT64) {
                *((int64_t*)opt_def->var) = num_result.val_int;
            }
            else {
                CH_VECTOR(word)* resultv = (CH_VECTOR(word)*)opt_def->var;
                if(opt_def->found == 1 && resultv->count == 1 ){ //This is the first option found
                    resultv->pop_back(resultv); //Remove the default value
                }
                resultv->push_back(resultv, num_result.val_int);
            }
            break;
        }

        case CH_UINT64:
        case CH_UINT64S:
        case CH_HEX:
        case CH_HEXS: {
            //Sanity check
            if (!optarg) { ch_opt_print_usage( "Option  --%s (-%c) Expected argument of type UINT64 but none found.\n", opt_def->long_opt, opt_def->short_opt); }

            //Get the number
            num_result = parse_number(optarg, 0);
            if (num_result.type != CH_UINT64) { ch_opt_print_usage("Option  --%s (-%c) Expected argument of type UINT64 but \"%s\" found\n", opt_def->long_opt, opt_def->short_opt, optarg);}

            //Assign it
            if (opt_def->type == CH_UINT64 || opt_def->type == CH_HEX){
                *((uint64_t*)opt_def->var) = num_result.val_uint;
            }
            else{
                CH_VECTOR(u64)* resultv = (CH_VECTOR(u64)*)opt_def->var;
                if(opt_def->found == 1 && resultv->count == 1 ){ //This is   the first option found
                    resultv->pop_back(resultv); //Remove the default value
                }
                resultv->push_back(resultv, num_result.val_uint);
            }
            break;
        }

        case CH_DOUBLE:
        case CH_DOUBLES: {
            //Sanity check
            if (!optarg) { ch_opt_print_usage("Option --%s (-%c) Expected argument of type DOUBLE but none found.\n", opt_def->long_opt, opt_def->short_opt);}

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
                ch_opt_print_usage("Option --%s (-%c) Expected argument of type DOUBLE but \"%s\" found\n", opt_def->long_opt, opt_def->short_opt, optarg);
            }

            //Assign it
            if (opt_def->type == CH_DOUBLE){
                *(double*) opt_def->var = result;
            }
            else{
                CH_VECTOR(float)* resultv = (CH_VECTOR(float)*)opt_def->var;
                if(opt_def->found == 1 && resultv->count == 1 ){ //This is the first option found
                    resultv->pop_back(resultv); //Remove the default value
                }
                resultv->push_back(resultv, result);
            }

            break;
        }

        case CH_BOOL:
        case CH_BOOLS: {
            //Sanity check
            if (!optarg) { ch_opt_print_usage("Option --%s (-%c) Expected argument of type BOOL but none found.\n", opt_def->long_opt, opt_def->short_opt);}

            //Get the number
            num_result = parse_bool(optarg, strlen(optarg), 0);
            if (num_result.type != CH_INT64) { ch_opt_print_usage("Option --%s (-%c) Expected argument of type BOOL but \"%s\" found\n", opt_def->long_opt, opt_def->short_opt,  optarg);}

            //Assign it
            if (opt_def->type == CH_BOOL){
                *((int*)opt_def->var) = (int) num_result.val_int;
            }
            else{
                CH_VECTOR(ch_bool)* resultv = (CH_VECTOR(ch_bool)*)opt_def->var;
                if(opt_def->found == 1 && resultv->count == 1 ){ //This is the first option found
                    resultv->pop_back(resultv); //Remove the default value
                }
                resultv->push_back(resultv, num_result.val_int);
            }

            break;
        }

        case CH_STRING:
        case CH_STRINGS: {
            //Sanity check
            if (!optarg) { ch_opt_print_usage("Option --%s (-%c) Expected argument of type STRING but none found.\n", opt_def->long_opt, opt_def->short_opt);}

            //Assign it
            if (opt_def->type == CH_STRING) {
                * ((char**)opt_def->var) = optarg;
            }
            else{
                CH_VECTOR(cstr)* resultv = (CH_VECTOR(cstr)*)opt_def->var;
                if(opt_def->found == 1 && resultv->count == 1 ){ //This is the first option found
                    resultv->pop_back(resultv); //Remove the default value
                }
                resultv->push_back(resultv, optarg);
            }

            break;
        }

        default: {
            ch_opt_print_usage("Option --%s (-%c) argument has unknown type (%s)\n", opt_def->long_opt, opt_def->short_opt, optarg);
            break;
        }
    }
}

//Search through the options, find the option with the matching character
void process_option(char c, const char* name) {

    //int done = 0;
    for (ch_options_opt_t* opt_def = opts.opt_defs->first;
            opt_def < opts.opt_defs->end;
            opt_def = opts.opt_defs->next(opts.opt_defs, opt_def) ) {

        if ( (opt_def->short_opt && opt_def->short_opt == c) || (!c && ((strcmp(name, opt_def->long_opt)) == 0))) {
            //done = 1; //Exit the loop when finished

            opt_def->found++; //We found an option of this type
            if (opt_def->found > 1 && !is_vector(opt_def->type)) {
                ch_opt_print_usage("Option --%s (-%c), only one instance expected but multiple found.\n", opt_def->long_opt, opt_def->short_opt);
            }

            if (opt_def->mode == CH_OPTION_FLAG) {
                *(bool*)opt_def->var = true; // ! (*(int*)opt_def->var); //invert the default value
                //printf("Set opt: %c = %p = %u\n", opt_def->short_opt, opt_def->var, *(bool*)opt_def->var);
                break;  //This is all handled by getopts
            }

            //Parse and assign the argument
            parse_argument(opt_def);
            break;
        }
    }

//    printf("****************\n");
//    for (ch_options_opt_t* opt_def = opts.opt_defs->first; opt_def < opts.opt_defs->end; opt_def = opts.opt_defs->next(opts.opt_defs, opt_def) ) {
//        printf("Set opt: %c = %p = %u\n", opt_def->short_opt, opt_def->var, *(bool*)opt_def->var);
//    }
}


//Fill in the getopts and getopts_long structures
void generate_unix_opts(char short_opts_str[1024], struct option* long_options) {
    size_t i = 0;

    char* short_opts_ptr = short_opts_str;

    for (ch_options_opt_t* opt_def = opts.opt_defs->first;
            opt_def < opts.opt_defs->end && short_opts_ptr < &short_opts_str[1024];
            opt_def = opts.opt_defs->next(opts.opt_defs, opt_def), i++ ) {

        if(opt_def->short_opt != 0)
        {
            *short_opts_ptr = opt_def->short_opt;
            short_opts_ptr++;
        }

        long_options[i].name = opt_def->long_opt;

        if (opt_def->mode == CH_OPTION_FLAG) {
            long_options[i].name = opt_def->long_opt;
            long_options[i].has_arg = no_argument;
            long_options[i].flag = opt_def->var;
            long_options[i].val = true; //!(*(int*) opt_def->var);
        } else {
            long_options[i].name = opt_def->long_opt;
            long_options[i].has_arg = required_argument;
            long_options[i].flag = NULL;
            long_options[i].val = opt_def->short_opt;

            if(opt_def->short_opt != 0)
            {
                *short_opts_ptr = ':';
                short_opts_ptr++;
            }
        }


    }
    long_options[i].flag = NULL;
    long_options[i].has_arg = 0;
    long_options[i].name = NULL;
    long_options[i].val = 0;
}

int ch_opt_parse(int argc, char** argv){
    ch_opt_addbi(CH_OPTION_FLAG, 'h', "help", "Print this help message", &opts.help, 0);

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
            ch_opt_print_usage(NULL);
            exit(0);
        }

        //Search through the options, find the option with the matching character
        if(c == '?'){
            ch_opt_print_usage("Unknown option or option is missing an argument \"%s\"\n", argv[optind -1]);
        }

        process_option(c, long_options[option_index].name);

    }

    if (optind < argc){ //There are extra parameters
        //Look for an opt_def with type UNLIMITED
        ch_options_opt_t* opt_def = NULL;
        for (opt_def = opts.opt_defs->first;
                opt_def < opts.opt_defs->end;
                opt_def = opts.opt_defs->next(opts.opt_defs, opt_def) ) {
            if(opt_def->mode == CH_OPTION_UNLIMTED){
                break;
            }
        }

        //None found
        if(opt_def >= opts.opt_defs->end){
            ch_opt_print_usage("Unknown option \"%s\"\n", argv[optind]);
        }

        //Parse the remaning options
        for(; optind < argc; optind++){
            optarg = argv[optind];
            opt_def->found++;
            parse_argument(opt_def);
        }
    }


    //Check the constraints
    for (ch_options_opt_t* opt_def = opts.opt_defs->first;
                           opt_def < opts.opt_defs->end;
                            opt_def = opts.opt_defs->next(opts.opt_defs, opt_def) ) {
        if(opt_def->mode == CH_OPTION_REQUIRED && opt_def->found < 1){
            ch_opt_print_usage("Option --%s (-%c) is required but not supplied\n", opt_def->long_opt, opt_def->short_opt);
        }
    }


    return 0;
}
