/*
 * term_color.c
 *
 *  Created on: Jul 3, 2013
 *      Author: mgrosvenor
 */


#include "../string/string.h"
#include "term_color.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


//Print to a file descriptor a colored formatted string
//return the number of bytes written
int dprintf_color_va(int fd, ch_colour_t color, ch_str format, va_list va)
{

    ch_str result = CH_STR_STACK("",1024);

    //Only print colors if we're talking to a TTY
    //Set the colour/attributes
    if(isatty(fd)){
        CH_STR_CAT_STACK(result,color);
    }


    //Format the text -- the bvformata is broken and I don't have time to figure out why
    ch_word size = 2048;
    ch_str text = CH_STR("", size);
    size = vsnprintf(CH_STR_CSTR(text),size,CH_STR_CSTR(format),va);
    text.slen = size;
    CH_STR_CAT_STACK(result,text);
    CH_STR_FREE(text);

    //Reset since we're done with the colour for the moment
    if(isatty(fd)){
        ch_str reset = CH_TERM_COL_NONE;
        CH_STR_CAT_STACK(result, reset);
    }

    //Output to file descriptor
    return write(fd, CH_STR_CSTR(result), result.slen);

}

int dprintf_color(int fd, ch_colour_t color, ch_str format, ...)
{
    va_list arg;
    va_start(arg,format);
    int result = dprintf_color(fd,color,format,arg);
    va_end(arg);
    return result;
}

int printf_color_va( ch_colour_t color, ch_str format, va_list va)
{
    return dprintf_color_va(STDOUT_FILENO,color,format,va);
}


int printf_color( ch_colour_t color, ch_str format, ...)
{
    va_list args;
    va_start(args, format);
    int result = dprintf_color_va(STDOUT_FILENO,color,format,args);
    va_end(args);
    return result;
}

