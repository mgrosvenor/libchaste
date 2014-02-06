// CamIO 2: perf_mon.c
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "../log/log.h"
#include "perf_mon.h"
#include "../parsing/utils.h"
#include "../utils/util.h"

typedef enum {
    PERF_TOK_TSC,            // TSC
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

typedef enum { AST_NODE_TSC, AST_NODE_PERF_STRUCT } ast_node_e;


//Define an "abstract syntax tree"
struct ast_node;
typedef struct ast_node ast_node_t;
struct ast_node {
    ast_node_e type;
    perf_mod_generic_t* perf_mod;
    ast_node_t* child;
    ast_node_t* next;
    ch_cstr ident;
    ch_word ident_len;
    ch_word num;
    union {
        TSC* tsc;
        perf_mod_generic_t* child_perf;
    };
};



typedef enum { PAR_STATE_TYPE, PAR_STATE_IDENT, PAR_STATE_OPENB, PAR_STATE_NUM, PAR_STATE_CLOSEB, PAR_STATE_SEMI, PAR_STATE_PTR } parser_state;
//State machine for parsing up the text into an AST
void perf_stats_parse(perf_mod_generic_t* perf_mod, ast_node_t** ast_head_out)
{

    if(!ast_head_out){
        ch_log_fatal("AST head pointer pointer is null. Cannot continue\n");
    }


    ast_node_t* ast_head = NULL;

    ch_cstr descr       = perf_mod->descr;
    ch_byte* data       = (ch_byte*)(perf_mod + 1);
    parser_state state  = PAR_STATE_TYPE;
    ch_bool found_tsc   = false;
    ch_bool found_perf  = false;
    ch_cstr ident_start = NULL;
    ch_cstr ident_end   = NULL;
    ch_word tsc_num     = 0;

    //Pull tokens out of the description string
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
                        state = PAR_STATE_PTR;
                        continue;
                    }
                    default:
                        goto exit_err;
                }
                break;
            }


            case PAR_STATE_PTR:{
                switch(tok.type){
                    case PERF_TOK_WSPACE:
                        continue;
                    case PERF_TOK_STAR: {
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

            case PAR_STATE_SEMI:{
                switch(tok.type){
                    case PERF_TOK_WSPACE:
                        continue;
                    case PERF_TOK_SEMI: {
                        //If we've got as far as a semicolon, we've successfully parsed an entire line. So add an entry into the ast

                        if(!ast_head){
                            ast_head  = (ast_node_t*)calloc(1,sizeof(ast_node_t));
                            *ast_head_out = ast_head;
                        }
                        else if (!ast_head->next){
                            ast_head->next = (ast_node_t*)calloc(1,sizeof(ast_node_t));
                            ast_head       = ast_head->next;
                        }

                        if(found_perf){
                            //printf("Found perf struct with ident = \"%.*s\"\n", (int)(ident_end - ident_start), ident_start );

                            ast_head->type      = AST_NODE_PERF_STRUCT;
                            ast_head->ident     = ident_start;
                            ast_head->ident_len = (ident_end - ident_start);
                            ast_head->num       = 1;
                            ast_head->perf_mod  = perf_mod;
                            ast_head->child_perf = (perf_mod_generic_t*)data;

                            //Recursive call, adds a branch to the tree
                            perf_stats_parse( (perf_mod_generic_t*)data, &ast_head->child);

                            ast_head->child_perf = (perf_mod_generic_t*)data;
                            data += sizeof(perf_mod_generic_t*);

                        }
                        else if(found_tsc){
                            //printf("Found TSC with ident = \"%.*s\"\n", (int)(ident_end - ident_start), ident_start );

                            ast_head->type      = AST_NODE_TSC;
                            ast_head->ident     = ident_start;
                            ast_head->ident_len = (ident_end - ident_start);
                            ast_head->num       = tsc_num;
                            ast_head->perf_mod  = perf_mod;
                            ast_head->child     = NULL;

                            ast_head->tsc = (TSC*)data;

                            //Move the pointer up to the end of this array
                            data += sizeof(TSC) * tsc_num;

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
        }
    }


    return;

    exit_err:
    switch(state){
        case PAR_STATE_TYPE:    printf("Error: expected TSC, but found %s.\n", tok.start); return;
        case PAR_STATE_IDENT:   printf("Error: expected identifier, but found %s.\n", tok.start); return;
        case PAR_STATE_SEMI:    printf("Error: expected ';', but found %s.\n", tok.start); return;
        case PAR_STATE_PTR:     printf("Error: expected '*', but found %s.\n", tok.start); return;
        case PAR_STATE_OPENB:   printf("Error: expected '[', but found %s.\n", tok.start); return;
        case PAR_STATE_NUM:     printf("Error: expected a number, but found %s.\n", tok.start); return;
        case PAR_STATE_CLOSEB:  printf("Error: expected ']', but found %s.\n", tok.start); return;
    }
}


//static int i = 0;
void print_ast(ast_node_t* head, ch_word indent)
{

    const ch_cstr whitespace = "                                    ";
    const ch_word whitespace_len = strlen(whitespace);


    if(head == NULL){
        return;
    }

    switch(head->type){
        case AST_NODE_TSC:{
            printf("%.*s%.*s\n", (int)MIN(whitespace_len,indent), whitespace, (int)head->ident_len, head->ident);
            printf("%.*s    Started: %li\n", (int)MIN(whitespace_len,indent), whitespace, head->tsc[0].start_count);
            for(ch_word i = 0; i < head->num; i++){
                head->tsc[i].nanos_avg = (double) head->tsc[i].nanos_total / (double)head->tsc[i].end_count;

                printf("%.*s    End %li: %li -  Nanos: [%li, %li < %6.2lf < %li], Cycles  [%li, %li < %6.2lf < %li]\n", (int)MIN(whitespace_len,indent), whitespace, i,
                                                                                    head->tsc[i].end_count,
                                                                                    head->tsc[i].nanos_total, head->tsc[i].nanos_min, head->tsc[i].nanos_avg, head->tsc[i].nanos_max,
                                                                                    head->tsc[i].cycles_total, head->tsc[i].cycles_min, head->tsc[i].cycles_avg, head->tsc[i].cycles_max);

            }
            if(head->next) print_ast(head->next, indent);
            free(head);
            return;
        }

        case AST_NODE_PERF_STRUCT:{
            printf("%.*s%.*s\n",  (int)MIN(whitespace_len,indent), whitespace,  (int)head->ident_len, head->ident);
            if(head->child) print_ast(head->child, indent + 1);
            if(head->next)  print_ast(head->next, indent);
            free(head);
            return;
        }
    }
}


void print_perf_stats(perf_mod_generic_t* perf_mod)
{
    ast_node_t* ast_head = NULL;
    perf_stats_parse(perf_mod,&ast_head);
    print_ast(ast_head,0);
}


u64 get_nanos_now()
{
    struct timespec ts = { 0 };
    clock_gettime(CLOCK_REALTIME,&ts);
    return ts.tv_nsec + ts.tv_sec * 1000 * 1000 * 1000;

}
