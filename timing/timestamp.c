/*
 * timestamp.c
 *
 *  Created on: Jul 3, 2013
 *      Author: mgrosvenor
 */

#include "timestamp.h"

#include <time.h>
#include "../deps/libbstring/bstraux.h"


//Generates an ISO 8601 formatted timestamp with a few options to play with
//See https://en.wikipedia.org/wiki/ISO_8601
m6_str generate_iso_timestamp(m6_bool use_gmt, m6_word subseconds, m6_bool incl_tz_offset)
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
    m6_str time_major = bStrfTime("%Y%m%dT%H%M%S",timeinfo);


    //Create the subseconds timestamp component with variable accuracy
    m6_str time_minor = NULL;
    if(subseconds > 0){
        time_minor = bformat(".%li", ts.tv_nsec);
        bassignmidstr(time_minor,time_minor,0,subseconds +1);
    }

    //Add the timezone offset to GMT
    m6_str time_offset = bfromcstr("");
    if(incl_tz_offset){
        time_offset = bStrfTime("%z",timeinfo);
    }

    m6_str iso_time_out = bfromcstr("");
    bconcat(iso_time_out, time_major);
    bstrFree(time_major); //Now done with time major

    bconcat(iso_time_out,time_minor);
    bstrFree(time_minor); //Now done with time_minor

    bconcat(iso_time_out,time_offset);
    bstrFree(time_offset); //Now done with time major

    return iso_time_out; //The caller is responsible for calling free

}

