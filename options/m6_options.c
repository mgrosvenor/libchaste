/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Tools to parse and print command line options
 */



#include <stdarg.h>
#include <getopt.h>

#include "camio_prog_options.h"

#include "../utils/camio_util.h"
#include "../errors/camio_errors.h"
#include "../types/camio_types.h"

#include "../parsing/numeric_parser.h"
#include "../parsing/bool_parser.h"


camio_options_t opts = {0};


void print_usage(const char* err_tx_fmt, ...){
    if(opts.short_description)
        printf("\n%s:\n\n", opts.short_description);


    camio_options_opt_t* opt_def = NULL;
    for (opt_def = opts.opt_def; opt_def ; opt_def = opt_def->next) {
        char* mode = NULL;
        switch(opt_def->mode){
            case CAMIO_OPTION_FLAG:      mode = "Flag"; break;
            case CAMIO_OPTION_REQUIRED:  mode = "Required"; break;
            case CAMIO_OPTION_OPTIONAL:  mode = "Optional"; break;
            case CAMIO_OPTION_UNLIMTED:  mode = "Unlimited"; break;
        }


        printf("%-9s -%c  --%-15s - %s\n", mode, opt_def->short_str, opt_def->long_str,  opt_def->descr);
    }

    if(opts.long_description)
        printf("%s\n\n", opts.long_description);

    if(err_tx_fmt){
        va_list args;
        va_start(args, err_tx_fmt);
        fprintf (stderr, "Error: ");
        vfprintf(stderr, err_tx_fmt, args);
        fprintf (stderr, "\n");
        va_end(args);
        exit(-1);
    }

    exit(0);
}



int camio_options_tail(char* description){
    eprintf_exit("");
    return 0;
}

int camio_options_short_description(char* description){
    opts.short_description = description;
    return 0;
}

int camio_options_long_description(char* description){
    opts.long_description= description;
    return 0;
}



int is_list_type(int type){
    return type == CAMIO_BOOLS   ||
           type == CAMIO_INT64S  ||
           type == CAMIO_STRINGS ||
           type == CAMIO_UINT64S ;
}


int camio_options_add(camio_options_mode_e mode, char short_str, const char* long_str, const char* descr, camio_types_e type, ...){

    //Create a new opt def
    camio_options_opt_t* opt_def_new = malloc(sizeof(camio_options_opt_t));
    if(!opt_def_new){
        eprintf_exit( "Could not allocate memory for option\n");
    }
    bzero(opt_def_new,sizeof(camio_options_opt_t));

    opts.count++;
    opt_def_new->mode      = mode;
    opt_def_new->short_str = short_str;
    opt_def_new->long_str  = long_str;
    opt_def_new->descr     = descr;
    opt_def_new->type      = type;

    if(mode == CAMIO_OPTION_FLAG && type != CAMIO_BOOL){
        eprintf_exit( "Flag options must be used with type CAMIO_BOOL\n");
    }

    if(mode == CAMIO_OPTION_UNLIMTED){
        if(!opts.unlimted_set){
            if(is_list_type(type)){
                opts.unlimted_set = 1;
            }
            else{
                eprintf_exit( "Unlimited options must be used with a list type variable eg camio_list(string)\n");
            }
        }
        else{
            eprintf_exit( "Unlimited options can only be used once!\n");
        }
    }

    //Populate it
    va_list argp;
    va_start(argp,type);
    switch(type){
        case CAMIO_BOOL:
            opt_def_new->var = va_arg(argp,int*);
            *((int*)opt_def_new->var) = va_arg(argp,int);
            break;
        case CAMIO_STRING:
            opt_def_new->var = va_arg(argp,char**);
            *((char**)opt_def_new->var) = va_arg(argp,char*);
            break;

        case CAMIO_INT64:
            opt_def_new->var = va_arg(argp,int64_t*);
            *((int64_t*)opt_def_new->var) = va_arg(argp,int64_t);
            break;

        case CAMIO_UINT64:
            opt_def_new->var = va_arg(argp,uint64_t*);
            *((uint64_t*)opt_def_new->var) = va_arg(argp,uint64_t);
            break;

        case CAMIO_DOUBLE:
            opt_def_new->var = va_arg(argp,double*);
            *((double*)opt_def_new->var) = va_arg(argp,double);
            break;

        //List types

        case CAMIO_BOOLS:
            opt_def_new->var = va_arg(argp,camio_list_t(bool)*);
            camio_list_init(bool,opt_def_new->var, 512);
            camio_list_add(bool,opt_def_new->var,va_arg(argp,int));
            break;

        case CAMIO_STRINGS:
            opt_def_new->var = va_arg(argp,camio_list_t(string)*);
            camio_list_init(string,opt_def_new->var, 512);
            camio_list_add(string,opt_def_new->var,va_arg(argp,char*));
            break;

        case CAMIO_INT64S:
            opt_def_new->var = va_arg(argp,camio_list_t(int64)*);
            camio_list_init(int64,opt_def_new->var, 512);
            camio_list_add(int64,opt_def_new->var,va_arg(argp,int64_t));
            break;

        case CAMIO_UINT64S:
            opt_def_new->var = va_arg(argp,camio_list_t(uint64)*);
            camio_list_init(uint64,opt_def_new->var, 512);
            camio_list_add(uint64,opt_def_new->var,va_arg(argp,uint64_t));
            break;

        case CAMIO_DOUBLES:
            opt_def_new->var = va_arg(argp,camio_list_t(double)*);
            camio_list_init(double,opt_def_new->var, 512);
            camio_list_add(double,opt_def_new->var,va_arg(argp,double));
            break;

        default:
            eprintf_exit( "Unknown type %lu for option %s\n", type, long_str);
            break;
    }

    va_end(argp);

    //And finally, add it to the end of the list
    if(!opts.opt_def){
        opts.opt_def = opt_def_new;
    }
    else{
        //Traverse to the end of the list
        camio_options_opt_t* opt_def_end = opts.opt_def;
        while(opt_def_end && opt_def_end->next){
            opt_def_end = opt_def_end->next;
        }

        opt_def_end->next = opt_def_new;
    }


    return 0;
}

//int64_t  is_number_int64     = 0;
//uint64_t is_number_uint64    = 0;
//double   is_number_double    = 0;



void parse_argument(camio_options_opt_t* opt_def) {
    num_result_t num_result;
    switch (opt_def->type) {
        case CAMIO_INT64:
        case CAMIO_INT64S: {
            //Sanity check
            if (!optarg) { print_usage( "Option --%s (-%c) Expected argument of type INT64 but none found.\n", opt_def->long_str, opt_def->short_str); }

            //Get the number
            num_result = parse_number(optarg, 0);
            if(num_result.type == CAMIO_UINT64){ num_result.type = CAMIO_INT64;  } //Type promote uint to int
            if (num_result.type != CAMIO_INT64) { print_usage( "Option --%s (-%c) Expected argument of type INT64 but \"%s\" found\n", opt_def->long_str, opt_def->short_str, optarg);}

            //Assign it
            if (opt_def->type == CAMIO_INT64) { *(int64_t*) opt_def->var = num_result.val_int;}
            else { camio_list_add(int64, opt_def->var, num_result.val_int); }
            break;
        }

        case CAMIO_UINT64:
        case CAMIO_UINT64S: {
            //Sanity check
            if (!optarg) { print_usage( "Option  --%s (-%c) Expected argument of type UINT64 but none found.\n", opt_def->long_str, opt_def->short_str); }

            //Get the number
            num_result = parse_number(optarg, 0);
            if (num_result.type != CAMIO_UINT64) { print_usage("Option  --%s (-%c) Expected argument of type UINT64 but \"%s\" found\n", opt_def->long_str, opt_def->short_str, optarg);}

            //Assign it
            if (opt_def->type == CAMIO_UINT64) { *(uint64_t*) opt_def->var = num_result.val_uint;
            } else { camio_list_add(uint64, opt_def->var, num_result.val_uint); }
            break;
        }

        case CAMIO_DOUBLE:
        case CAMIO_DOUBLES: {
            //Sanity check
            if (!optarg) { print_usage("Option --%s (-%c) Expected argument of type DOUBLE but none found.\n", opt_def->long_str, opt_def->short_str);}

            //Get the number
            num_result = parse_number(optarg, 0);
            double result = 0;
            if(num_result.type == CAMIO_DOUBLE){
                result = num_result.val_dble;
            }
            //Type promote int to double
            else if(num_result.type == CAMIO_UINT64){
                result = (double)num_result.val_int;
            }
            //Type promote uint to double
            else if(num_result.type == CAMIO_UINT64){
                result = (double)num_result.val_uint;
            }
            //No Suitable type found
            else{
                print_usage("Option --%s (-%c) Expected argument of type DOUBLE but \"%s\" found\n", opt_def->long_str, opt_def->short_str, optarg);
            }

            //Assign it
            if (opt_def->type == CAMIO_DOUBLE) { *(double*) opt_def->var = result;}
            else { camio_list_add(double, opt_def->var, num_result.val_dble); }
            break;
        }

        case CAMIO_BOOL:
        case CAMIO_BOOLS: {
            //Sanity check
            if (!optarg) { print_usage("Option --%s (-%c) Expected argument of type BOOL but none found.\n", opt_def->long_str, opt_def->short_str);}

            //Get the number
            num_result = parse_bool(optarg, strlen(optarg), 0);
            if (num_result.type != CAMIO_INT64) { print_usage("Option --%s (-%c) Expected argument of type BOOL but \"%s\" found\n", opt_def->long_str, opt_def->short_str,  optarg);}

            //Assign it
            if (opt_def->type == CAMIO_BOOL) { *(int*) opt_def->var = (int) num_result.val_int;}
            else {camio_list_add(bool, opt_def->var, (int)num_result.val_int);}
            break;
        }

        case CAMIO_STRING:
        case CAMIO_STRINGS: {
            //Sanity check
            if (!optarg) { print_usage("Option --%s (-%c) Expected argument of type STRING but none found.\n", opt_def->long_str, opt_def->short_str);}

            //Assign it
            if (opt_def->type == CAMIO_STRING) {
                *(char**) opt_def->var = optarg;
            }
            else {
                //Remove the default value
                if(opt_def->found == 1){
                    ((camio_list_t(string)*)opt_def->var)->count = 0;
                }

                camio_list_add(string, opt_def->var, optarg);
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
void process_option(const camio_options_t* opts, char c) {

    int done = 0;
    camio_options_opt_t* opt_def = NULL;
    for (opt_def = opts->opt_def; opt_def && !done; opt_def = opt_def->next) {

        if (opt_def->short_str == c) {
            done = 1; //Exit the loop when finished

            opt_def->found++; //We found an option of this type
            if (opt_def->found > 1 && !is_list_type(opt_def->type)) {
                print_usage("Option --%s (-%c), only one instance expected but multiple found.\n", opt_def->long_str, opt_def->short_str);
            }

            if (opt_def->mode == CAMIO_OPTION_FLAG) {
                *(int*)opt_def->var = ! (*(int*)opt_def->var); //invert the default value
                continue;  //This is all handled by getopts
            }

            //Parse and assign the argument
            parse_argument(opt_def);
        }
    }
}


//Fill in the getopts and getopts_long structures
void generate_unix_opts(const camio_options_t* opts, char short_opts_str[1024], struct option* long_options) {
    size_t i = 0;
    camio_options_opt_t* opt_def = opts->opt_def;
    char* short_opts_ptr = short_opts_str;
    for (; i < opts->count && opt_def && short_opts_ptr < &short_opts_str[1024]; i++) {
        *short_opts_ptr = opt_def->short_str;
        short_opts_ptr++;

        long_options[i].name = opt_def->long_str;

        if (opt_def->mode == CAMIO_OPTION_FLAG) {
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

        opt_def = opt_def->next;

    }
    long_options[i].flag = NULL;
    long_options[i].has_arg = 0;
    long_options[i].name = NULL;
    long_options[i].val = 0;
}

int camio_options_parse(int argc, char** argv){
    camio_options_add(CAMIO_OPTION_FLAG, 'h', "help", "Print this help message\n", CAMIO_BOOL, &opts.help, 0);

    char short_opts_str[1024] = {0};

    //Disable internal error reporting
    opterr = 0;

    //Form the getopts string
    struct option* long_options = (struct option*)malloc((opts.count + 1) * sizeof(struct option) );
    bzero(long_options, (opts.count + 1) * sizeof(struct option));

    generate_unix_opts(&opts, short_opts_str, long_options);

    int option_index = 0;
    char c = getopt_long (argc, argv, short_opts_str, long_options, &option_index);
    for(; c != EOF; c = getopt_long(argc, argv, short_opts_str, long_options, &option_index)){

        if( c == 'h' || opts.help){
            print_usage(NULL);
            exit(0);
        }

        //Search through the options, find the option with the matching character
        if(c == '?'){
            print_usage("Unknown option or missing argument \"%s\"\n", argv[optind -1]);
        }

        process_option(&opts, c);

    }

    if (optind < argc){ //There are extra parameters
        //Look for an opt_def with type UNLIMITED
        camio_options_opt_t* opt_def = NULL;
        for (opt_def = opts.opt_def; opt_def ; opt_def = opt_def->next) {
            if(opt_def->mode == CAMIO_OPTION_UNLIMTED){
                break;
            }
        }

        //None found
        if(opt_def == NULL){
            print_usage("Unknown option %s\n", argv[optind]);
        }

        //Parse the remaning options
        for(; optind < argc; optind++){
            optarg = argv[optind];
            opt_def->found++;
            parse_argument(opt_def);
        }
    }


    //Check the constraints
    camio_options_opt_t* opt_def = NULL;
    for (opt_def = opts.opt_def; opt_def ; opt_def = opt_def->next) {
        if(opt_def->mode == CAMIO_OPTION_REQUIRED && opt_def->found < 1){
            print_usage("Option --%s (-%c) is required but not supplied\n", opt_def->long_str, opt_def->short_str);
        }
    }


    return 0;
}
