/*
 * timestamp.c
 *
 *  Created on: Jul 3, 2013
 *      Author: mgrosvenor
 */

#include "timestamp.h"
#include "../string/string.h"
#include "../utils/util.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "../sys/sys.h"


//Generates an ISO 8601 formatted timestamp with a few options to play with
//See https://en.wikipedia.org/wiki/ISO_8601
ch_str generate_iso_timestamp(ch_bool use_gmt, ch_word subseconds, ch_bool incl_tz_offset)
{
    //Grab the time from the realtime clock and get it into a time_t for formatting
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);
    time_t rawtime = ts.tv_sec;
    struct tm * timeinfo;


    //Extract the major part of the timestamp as either local or UTC
    if(use_gmt){
        timeinfo = gmtime(&rawtime);
    }
    else{
        timeinfo = localtime(&rawtime);
    }

    //Create a string containing the timestamp major part
    ch_str time_major = CH_STR("",256);
    CH_STR_LEN(time_major) = strftime( CH_STR_CSTR_END(time_major), CH_STR_AVAIL(time_major), "%Y%m%dT%H%M%S",timeinfo);



    //Create the subseconds timestamp component with variable accuracy
    ch_str time_minor = CH_STR("",256);
    if(subseconds > 0){
        CH_STR_LEN(time_minor) += snprintf(CH_STR_CSTR_END(time_minor), CH_STR_AVAIL(time_minor),".%09li", ts.tv_nsec);
        ch_str_trunc(&time_minor, MAX(9 - subseconds, 0));
    }

    //Add the timezone offset to GMT
    ch_str time_offset = CH_STR("", 256);
    if(incl_tz_offset){
        CH_STR_LEN(time_offset) += strftime(CH_STR_CSTR_END(time_offset), CH_STR_AVAIL(time_offset), "%z",timeinfo );
    }

    ch_str iso_time_out = CH_STR("", 256);
    iso_time_out = CH_STR_CAT(&iso_time_out, time_major);
    ch_str_free(&time_major); //Now done with time major

    iso_time_out = CH_STR_CAT(&iso_time_out,time_minor);
    ch_str_free(&time_minor); //Now done with time_minor

    iso_time_out = CH_STR_CAT(&iso_time_out,time_offset);
    ch_str_free(&time_offset); //Now done with time major

    return iso_time_out; //The caller is responsible for calling free

}

