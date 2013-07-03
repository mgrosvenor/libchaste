/*
 * term_color.c
 *
 *  Created on: Jul 3, 2013
 *      Author: mgrosvenor
 */


#include "term_color.h"

#include <stdio.h>
#include <unistd.h>


//Print to a file descriptor a colored formatted string
//return the number of bytes written
int dprintf_color_va(int fd, m6_colour_t color, m6_str format, va_list va)
{

    m6_str result = bfromcstr("");

    //Only print colors if we're talking to a TTY
    //Set the colour/attributes
    if(isatty(fd)){
        bcatcstr(result,color);
    }


    //Format the text -- the bvformata is broken and I don't have time to figure out why
    int size = snprintf(NULL,0,cstr(format),va);
    m6_str text = bfromcstralloc(size,"");
    size = vsprintf(cstr(text),cstr(format),va); //XXX BUG! HACK!: Weird stuff happens here, the size changes! WTF??
    text->slen = size;
    bconcat(result,text);
    bstrFree(text);

    //Reset since we're done with the colour for the moment
    if(isatty(fd)){
        bcatcstr(result,M6_TERM_COL_NONE);
    }

    //Output to file descriptor
    return write(fd,cstr(result), result->slen);

}

int dprintf_color(int fd, m6_colour_t color, m6_str format, ...)
{
    va_list arg;
    va_start(arg,format);
    int result = dprintf_color(fd,color,format,arg);
    va_end(arg);
    return result;
}

int printf_color_va( m6_colour_t color, m6_str format, va_list va)
{
    return dprintf_color_va(STDOUT_FILENO,color,format,va);
}


int printf_color( m6_colour_t color, m6_str format, ...)
{
    va_list args;
    va_start(args, format);
    int result = dprintf_color_va(STDOUT_FILENO,color,format,args);
    va_end(args);
    return result;
}

