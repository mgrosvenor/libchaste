/*
 * numeric_parser.c
 *
 *  Created on: Dec 20, 2012
 *      Author: mgrosvenor
 */

#include "numeric_parser.h"
#include "utils.h"
#include "ctype.h"
#include "../log/log.h"


enum {
    STATE_INIT = 0,
    STATE_FOUND_SIGN,
    STATE_MUST_BE_ZERO_OR_PERIOD,
    STATE_FOUND_INIT_PERIOD,
    STATE_FOUND_INIT_ZERO,
    STATE_GET_DEC_DIGITS,
    STATE_GET_BIN_DIGITS,
    STATE_GET_OCT_DIGITS,
    STATE_GET_HEX_DIGITS,
    STATE_GET_FLO_DIGITS,
    STATE_END_UINT,
    STATE_END_INT,
    STATE_END_FLOAT,
    STATE_FINISHED_UINT,
    STATE_FINISHED_INT,
    STATE_FINISHED_FLOAT,


    STATE_NONE_FOUND
};




static inline int isprefix(char c){
    return( c == 'k' ||
            c == 'K' ||
            c == 'm' ||
            c == 'M' ||
            c == 'm' ||
            c == 'g' ||
            c == 'G' );
}


static inline int isbin(char c){
    return c == 'i' || c == 'I';
}

static inline int issci(char c){
    return c == 'E';
}


static inline int isnull(char c){
    return c == '\0';
}


static inline uint64_t get_prefix(char c){
    switch(c){
        case 'k':
        case 'K': return 1000ULL;
        case 'm':
        case 'M': return 1000ULL * 1000;
        case 'g':
        case 'G': return 1000ULL * 1000 * 1000;
    }

    return 0;
}



static inline uint64_t get_bin_prefix(char c){
    switch(c){
        case 'k':
        case 'K': return 1ULL << 10;
        case 'm':
        case 'M': return 1ULL << 20;
        case 'g':
        case 'G': return 1ULL << 30;
    }

    return 0;
}




//static inline
num_result_t parse_number(const char* c, size_t i){
    num_result_t num_result;
    num_result.type = CH_NO_TYPE;
    if( c[i] == '\0'){
        return num_result;
    }

    uint64_t uint_accumulator       = 0;
    int64_t int_accumulator         = 0;
    double float_accumulator        = 0;
    double float_base_accumulator   = 1;
    int64_t sign                    = 1;
    char prefix                     = 0;
    ch_word index                   = 0;
    size_t state = STATE_INIT;
    for(    ;
            state != STATE_NONE_FOUND &&
            state != STATE_FINISHED_INT &&
            state != STATE_FINISHED_UINT &&
            state != STATE_FINISHED_FLOAT;
            i++ ){

        index = i;

        switch(state){
            case STATE_INIT:{
                uint_accumulator        = 0;
                float_accumulator       = 0;
                float_base_accumulator  = 1;
                sign                    = 1;

                if( c[i] == '-')    { sign  = -1;
                                      state = STATE_FOUND_SIGN;             continue; }
                if( c[i] == '+')    { state = STATE_FOUND_SIGN;             continue; }
                if( c[i] == '.')    { state = STATE_GET_FLO_DIGITS;         continue; }
                if( c[i] == '0')    { state = STATE_FOUND_INIT_ZERO;        continue; }
                if( isdigit(c[i]) ) { uint_accumulator = (c[i] - '0');
                                      state = STATE_GET_DEC_DIGITS;         continue; }
                if( iswhite(c[i]) ) { state = STATE_INIT;                   continue; }
                else                { state = STATE_NONE_FOUND;             continue; }
            }

            case STATE_FOUND_SIGN:{
                if( c[i] == '0' )    { state = STATE_MUST_BE_ZERO_OR_PERIOD; continue; }
                if( c[i] == '.' )    { state = STATE_GET_FLO_DIGITS;         continue; }
                if( isdigit(c[i]) )  { uint_accumulator = (c[i] - '0');
                                       state = STATE_GET_DEC_DIGITS;         continue; }
                else                 { state = STATE_NONE_FOUND;             continue; }
            }

            case STATE_MUST_BE_ZERO_OR_PERIOD: {
                if( c[i] == '0' )    { state = STATE_MUST_BE_ZERO_OR_PERIOD; continue; }
                if( c[i] == '.' )    { state = STATE_GET_FLO_DIGITS;         continue; }
                else                 { state = STATE_NONE_FOUND;             continue; }

            }

            case STATE_FOUND_INIT_ZERO: {
                if( c[i] == 'x')    { state = STATE_GET_HEX_DIGITS;          continue; }
                if( c[i] == 'X')    { state = STATE_GET_HEX_DIGITS;          continue; }
                if( c[i] == 'b')    { state = STATE_GET_BIN_DIGITS;          continue; }
                if( c[i] == 'B')    { state = STATE_GET_BIN_DIGITS;          continue; }
                if( c[i] == '.')    { state = STATE_GET_FLO_DIGITS;          continue; }
                if( c[i] == '\0')   { state = STATE_FINISHED_UINT;           continue; }
                if( isprefix(c[i])) { prefix = c[i];
                                      state = STATE_END_UINT;                continue; }
                if( isodigit(c[i])) { uint_accumulator = (c[i] - '0');
                                      state = STATE_GET_OCT_DIGITS;          continue; }
                else                { state = STATE_NONE_FOUND;              continue; }
            }

            case STATE_GET_DEC_DIGITS:{
                if( isdigit(c[i]) ) { uint_accumulator *= 10;
                                      uint_accumulator += c[i] - '0';
                                      state = STATE_GET_DEC_DIGITS;         continue; }
                if( c[i] == '.')    { float_accumulator = (double)uint_accumulator;
                                      state = STATE_GET_FLO_DIGITS;         continue; }
                if( isprefix(c[i])) { prefix = c[i];
                                      state = STATE_END_UINT;               continue; }
                if( issci(c[i]) )
                if( iswhite(c[i]) ) { state = STATE_FINISHED_UINT;          continue; }
                if( isnull(c[i]) )  { state = STATE_FINISHED_UINT;          continue; }
                else                { state = STATE_NONE_FOUND;             continue; }
            }

            case STATE_GET_BIN_DIGITS: {
                if( isbdigit(c[i]) ) { uint_accumulator <<= 1;
                                       uint_accumulator += c[i] - '0';
                                       state = STATE_GET_BIN_DIGITS;        continue; }
                if( isprefix(c[i]))  { prefix = c[i];
                                       state = STATE_END_UINT;              continue; }
                if( iswhite(c[i]) )  { state = STATE_FINISHED_UINT;         continue; }
                if( isnull(c[i]) )   { state = STATE_FINISHED_UINT;         continue; }
                else                 { state = STATE_NONE_FOUND;            continue; }

            }

            case STATE_GET_OCT_DIGITS: {
                if( isodigit(c[i]) ) { uint_accumulator *= 8;
                                       uint_accumulator += c[i] - '0';
                                       state = STATE_GET_OCT_DIGITS;        continue; }
                if( isprefix(c[i]))  { prefix = c[i];
                                       state = STATE_END_UINT;              continue; }
                if( iswhite(c[i]) )  { state = STATE_FINISHED_UINT;         continue; }
                if( isnull(c[i]) )   { state = STATE_FINISHED_UINT;         continue; }
                else                 { state = STATE_NONE_FOUND;            continue; }
            }

            case STATE_GET_HEX_DIGITS: {
                if( isdigit(c[i]) )  { uint_accumulator *= 16;
                                       uint_accumulator += c[i] - '0';
                                       state = STATE_GET_HEX_DIGITS;        continue; }
                if( isxdigit(c[i]) ) { uint_accumulator *= 16;
                                       uint_accumulator += getxdigit(c[i]);
                                       state = STATE_GET_HEX_DIGITS;        continue; }
                if( isprefix(c[i]))  { prefix = c[i];
                                       state = STATE_END_UINT;              continue; }
                if( iswhite(c[i]) )  { state = STATE_FINISHED_UINT;         continue; }
                if( isnull(c[i]) )   { state = STATE_FINISHED_UINT;         continue; }

                else                 { state = STATE_NONE_FOUND;            continue; }
            }

            case STATE_GET_FLO_DIGITS: {
                if( isdigit(c[i]) )  { float_base_accumulator  *= 10.0;
                                       float_accumulator += (double)(c[i] - '0') / float_base_accumulator;
                                       state = STATE_GET_FLO_DIGITS;         continue; }
                if( isprefix(c[i]))  { prefix = c[i];
                                       state = STATE_END_FLOAT;              continue; }
                if( iswhite(c[i])  ) { state = STATE_END_FLOAT;              continue; }
                if( isnull(c[i]) )   { state = STATE_FINISHED_FLOAT;         continue; }
                else                 { state = STATE_NONE_FOUND;             continue; }
            }

            case STATE_END_UINT:{
                if( isbin( c[i]) &&
                    isprefix(prefix)) { uint_accumulator *= get_bin_prefix(prefix);
                                        index += 1;
                                        state = STATE_FINISHED_UINT;        continue; }
                if( isprefix(prefix)) { uint_accumulator *= get_prefix(prefix);
                                        state = STATE_FINISHED_UINT;        continue; }
                if( iswhite(c[i]) )   { state = STATE_FINISHED_UINT;        continue; }
                if( isnull(c[i]) )    { state = STATE_FINISHED_UINT;        continue; }
                else                  { state = STATE_NONE_FOUND;           continue; }
            }

            case STATE_END_INT:  {
                if( isbin(c[i]) &&
                    isprefix(prefix)) { int_accumulator *= get_bin_prefix(prefix);
                                        index += 1;
                                        state = STATE_FINISHED_INT;          continue; }
                if( isprefix(prefix)) { int_accumulator *= get_prefix(prefix);
                                        state = STATE_FINISHED_INT;          continue; }
                if( iswhite(c[i]) )   { state = STATE_FINISHED_INT;          continue; }
                if( isnull(c[i]) )    { state = STATE_FINISHED_INT;          continue; }
                else                  { state = STATE_NONE_FOUND;            continue; }
            }

            case STATE_END_FLOAT: {
                if( isbin( c[i] &&
                    isprefix(prefix))) { float_accumulator *= get_bin_prefix(prefix);
                                         index += 1;
                                         state = STATE_FINISHED_FLOAT;       continue; }
                if( isprefix(prefix))  { float_accumulator *= get_prefix(prefix);
                                         state = STATE_FINISHED_FLOAT;       continue; }
                if( isnull(c[i]) )     { state = STATE_FINISHED_FLOAT;       continue; }
                if( iswhite(c[i]) )    { state = STATE_FINISHED_FLOAT;       continue; }
                else                   { state = STATE_NONE_FOUND;           continue; }
            }
            default:{
                ch_log_error("Undefined state parsing numeric %lu\n", state);
                num_result.type = CH_NO_TYPE;
                return num_result;
            }

        }
    }


    switch(state){
        case STATE_NONE_FOUND:{
            num_result.type     = CH_NO_TYPE;
            break;
        }

        case STATE_FINISHED_INT: {
            num_result.type     = CH_INT64;
            num_result.val_int  = int_accumulator;
            break;
        }
        case STATE_FINISHED_UINT:{
            if(sign == -1){
                int_accumulator     = uint_accumulator * sign;
                num_result.type     = CH_INT64;
                num_result.val_int  = int_accumulator;
                break;
            }

            num_result.type     = CH_UINT64;
            num_result.val_uint = uint_accumulator;
            break;
        }
        case STATE_FINISHED_FLOAT:{
            num_result.type     = CH_DOUBLE;
            num_result.val_dble = float_accumulator * (double)sign;
            break;
        }
        default:{
            ch_log_error("Undefined state parsing numeric %lu\n", state);
            num_result.type = CH_NO_TYPE;
            return num_result;
        }
    }

    if(num_result.type != CH_NO_TYPE){
        num_result.index = index;
    }

    return num_result;


}
