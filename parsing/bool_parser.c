#include "bool_parser.h"
#include "utils.h"

num_result_t parse_bool(const char* c, size_t n, size_t i){
    num_result_t num_result;
    num_result.type =  CAMIO_NO_TYPE;

    while( iswhite(c[i]) ){i++;}

    if(c[i] == '\0'){
        return num_result;
    }

    if(c[i] == '1' ||
       c[i] == 'T' ||
       c[i] == 't' ){
        num_result.type = CAMIO_INT64;
        num_result.val_int = 1;
        return num_result;
    }

    if(c[i] == '0' ||
       c[i] == 'F' ||
       c[i] == 'f' ){
        num_result.type = CAMIO_INT64;
        num_result.val_int = 0;
        return num_result;
    }

    return num_result;

}

