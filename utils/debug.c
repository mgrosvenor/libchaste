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
#include <ctype.h>
#include <string.h>

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
        case ERR:   mode_str = "Error"; break;
        case DBG:   mode_str = "Debug"; break;
        case WARN:  mode_str = "Warning:"; break;
    }
    if(info) dprintf(OUTPUT_TO,"[%s - %s:%i:%s()]  ", mode_str, basename(fn), (int)line_num, function);
    ch_word result = vdprintf(OUTPUT_TO,format,args);
    va_end(args);

    return result;
}

void hexdump(void *data, int size)
{
    /* dumps size bytes of *data to stdout. Looks like:
     * [0000] 75 6E 6B 6E 6F 77 6E 20 30 FF 00 00 00 00 39 00 unknown 0.....9.
     */

    unsigned char *p = data;
    unsigned char c;
    int n;
    char bytestr[4] = {0};
    char addrstr[10] = {0};
    char hexstr[ 16*3 + 5] = {0};
    char charstr[16*1 + 5] = {0};
    for(n=1;n<=size;n++) {
        if (n%16 == 1) {
            /* store address for this line */
            snprintf(addrstr, sizeof(addrstr), "%.4lx", (size_t)(p-(unsigned char*)data));
        }

        c = *p;
        if (isalnum(c) == 0) {
            c = '.';
        }

        /* store hex str (for left side) */
        snprintf(bytestr, sizeof(bytestr), "%02X ", *p);
        strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);

        /* store char str (for right side) */
        snprintf(bytestr, sizeof(bytestr), "%c", c);
        strncat(charstr, bytestr, sizeof(charstr)-strlen(charstr)-1);

        if(n%16 == 0) {
            /* line completed */
            DBG2("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
            hexstr[0] = 0;
            charstr[0] = 0;
        } else if(n%8 == 0) {
            /* half line: add whitespaces */
            strncat(hexstr, "  ", sizeof(hexstr)-strlen(hexstr)-1);
            strncat(charstr, " ", sizeof(charstr)-strlen(charstr)-1);
        }
        p++; /* next byte */
    }

    if (strlen(hexstr) > 0) {
        /* print rest of buffer if not empty */
        DBG2("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
    }
}
