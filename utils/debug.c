/*
 *  Created:   15 Oct 2014
 *  File name: debug.c
 *  Description:
 *  A very simple debug printer
 */

#include <stdio.h>
#include <stdarg.h>
#include <libgen.h>
#include <unistd.h>

#include "debug.h"

//******************************************//
//Just for debugging

#define OUTPUT_TO STDOUT_FILENO
ch_word ch_debug_out_(
        ch_bool info,
        ch_dbg_mode_e mode,
        ch_word line_num,
        const char* filename,
        const char* function,
        const char* format, ... ) //Intentionally using char* here as these are passed in as constants
{
    va_list args;
    va_start(args,format);
    char* fn =  (char*)filename;
    char* mode_str = NULL;
    switch(mode){
        case ERROR: mode_str = "Fatal Error"; break;
        case DBG:   mode_str = "Debug"; break;
        case WARN:  mode_str = "Warning:"; break;
    }
    if(info) dprintf(OUTPUT_TO,"[%s - %s:%i:%s()]  ", mode_str, basename(fn), (int)line_num, function);
    ch_word result = vdprintf(OUTPUT_TO,format,args);
    va_end(args);

    return result;
}
