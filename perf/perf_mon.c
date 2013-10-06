// CamIO 2: perf_mon.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include <string.h>
#include <stdio.h>
#include <ctype.h>


#include "perf_mon.h"
#include "../parsing/utils.h"

typedef enum { PERF_TOK_TSC,            // TSC
    PERF_TOK_STAR,           // *
    PERF_TOK_PERF_STRUCT,    //
    PERF_TOK_IDENT,          // An identifier
    PERF_TOK_WSPACE,         // Any white space
    PERF_TOK_SEMI,           // ;
    PERF_TOK_NULL,           // \0
    PERF_TOK_OPENB,          // [
    PERF_TOK_CLOSEB,         // ]
    PERF_TOK_NUMBER,         //
    PERF_TOK_UNKNOWN         //Uh oh....
} tok_type_e;


typedef struct {
    ch_cstr start;
    ch_cstr end;
    tok_type_e type;
    ch_word num;
} tok_t;


//Tokenize the TSC structure description string
tok_t get_tsc_tok(ch_cstr c)
{
    tok_t result = { .start = c, .end = c, .type = PERF_TOK_UNKNOWN, .num = 0 };

    if(*c == '\0'){
        result.type = PERF_TOK_NULL;
        return result;
    }

    while(iswhite(*c)){
        c++;
    }
    //Chomp off whitespace
    if(result.start != c){
        result.end = c;
        result.type = PERF_TOK_WSPACE;
        return result;
    }

    if(*c == ';'){
        result.end += 1;
        result.type = PERF_TOK_SEMI;
        return result;
    }

    if(*c == '*'){
        result.end += 1;
        result.type = PERF_TOK_STAR;
        return result;
    }

    if(*c == '['){
        result.end += 1;
        result.type = PERF_TOK_OPENB;
        return result;
    }

    if(*c == ']'){
        result.end += 1;
        result.type = PERF_TOK_CLOSEB;
        return result;
    }

    if(strncmp("TSC",c,3) == 0){
        result.end += 3;
        result.type = PERF_TOK_TSC;
        return result;
    }

    if(isdigit(*c)){
        result.num = *c - '0';
        c++;
        while(isdigit(*c)){
            result.num *= 10;
            result.num += *c - '0';
            c++;
        }
    }
    if(result.start != c){
        result.end = c;
        result.type = PERF_TOK_NUMBER;
        return result;
    }


    while(!iswhite(*c) && *c != '[' && *c != ';'){
        c++;
    }
    result.end = c;
    result.type = PERF_TOK_IDENT;
    return result;

}

typedef enum { PAR_STATE_TYPE, PAR_STATE_IDENT, PAR_STATE_OPENB, PAR_STATE_NUM, PAR_STATE_CLOSEB, PAR_STATE_SEMI } parser_state;

void print_perf_stats_recursive(perf_mod_generic_t* perf_mod, ch_word indent)
{
    ch_cstr descr = perf_mod->descr;
    parser_state state  = PAR_STATE_TYPE;
    ch_bool found_tsc   = false;
    ch_bool found_perf  = false;
    ch_cstr ident_start = NULL;
    ch_cstr ident_end   = NULL;
    ch_word tsc_num     = 0;

    tok_t tok;
    for(tok = get_tsc_tok(descr); tok.type != PERF_TOK_NULL; descr = tok.end, tok = get_tsc_tok(descr) ){

        switch(state){
            case PAR_STATE_TYPE:{
                switch(tok.type){
                    case PERF_TOK_WSPACE:
                        continue;
                    case PERF_TOK_TSC: {
                        found_tsc = true;
                        state = PAR_STATE_IDENT;
                        continue;
                    }
                    case PERF_TOK_PERF_STRUCT:{
                        found_perf = true;
                        state = PAR_STATE_IDENT;
                        continue;
                    }
                    default:
                        goto exit_err;
                }
                break;
            }

            case PAR_STATE_IDENT:{
                switch(tok.type){
                    case PERF_TOK_WSPACE: continue;
                    case PERF_TOK_IDENT:{
                        if(found_tsc){
                            ident_start = tok.start;
                            ident_end = tok.end;
                            state = PAR_STATE_OPENB;
                            continue;
                        }

                        if(found_perf){
                            ident_start = tok.start;
                            ident_end = tok.end;
                            state = PAR_STATE_SEMI;
                            continue;
                        }

                        goto exit_err;
                    }
                    default:
                        goto exit_err;
                }
                break;
            }

            case PAR_STATE_SEMI:{
                switch(tok.type){
                    case PERF_TOK_WSPACE:
                        continue;
                    case PERF_TOK_SEMI: {
                        if(found_perf){
                            printf("Found perf struct with ident = \"%.*s\"\n", (int)(ident_end - ident_start), ident_start );
                            indent += 4;
                            print_perf_stats_recursive(perf_mod, indent); //This is wrong
                        }

                        if(found_tsc){
                            printf("Found TSC with ident = \"%.*s\"\n", (int)(ident_end - ident_start), ident_start );
                        }

                        state       = PAR_STATE_TYPE;
                        found_tsc   = false;
                        found_perf  = false;
                        ident_start = NULL;
                        ident_end   = NULL;

                        continue;
                    }
                    default:
                        goto exit_err;
                }
                break;
            }

            case PAR_STATE_OPENB:{
                switch(tok.type){
                    case PERF_TOK_WSPACE:
                        continue;
                    case PERF_TOK_OPENB: {
                        state = PAR_STATE_NUM;
                        continue;
                    }
                    default:
                        goto exit_err;
                }
                break;
            }

            case PAR_STATE_NUM:{
                switch(tok.type){
                    case PERF_TOK_WSPACE:
                        continue;
                    case PERF_TOK_NUMBER: {
                        tsc_num = tok.num;
                        state = PAR_STATE_CLOSEB;
                        continue;
                    }
                    default:
                        goto exit_err;
                }
                break;
            }


            case PAR_STATE_CLOSEB:{
                switch(tok.type){
                    case PERF_TOK_WSPACE:
                        continue;
                    case PERF_TOK_CLOSEB: {
                        state = PAR_STATE_SEMI;
                        continue;
                    }
                    default:
                        goto exit_err;
                }
                break;
            }

        }
    }

    return;

    exit_err:
    switch(state){
        case PAR_STATE_TYPE:    printf("Error: expected TSC, but found %s.\n", tok.start); return;
        case PAR_STATE_IDENT:   printf("Error: expected identifier, but found %s.\n", tok.start); return;
        case PAR_STATE_SEMI:    printf("Error: expected ';', but found %s.\n", tok.start); return;
        case PAR_STATE_OPENB:   printf("Error: expected '[', but found %s.\n", tok.start); return;
        case PAR_STATE_NUM:     printf("Error: expected a number, but found %s.\n", tok.start); return;
        case PAR_STATE_CLOSEB:  printf("Error: expected ']', but found %s.\n", tok.start); return;
    }
}



void print_perf_stats(perf_mod_generic_t* perf_mod)
{
    print_perf_stats_recursive(perf_mod,0);
}
