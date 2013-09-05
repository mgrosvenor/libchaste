/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Input stream factory definition
 *
 */

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#include "perf.h"
#include "../utils/util.h"
#include "../log/log.h"


void ch_perf_open_output(ch_perf_output_e output,const char* filename)
{
    switch(output){
        case ch_perf_output_tofile: {
            ch_perf.fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
            if(ch_perf.fd < 0){
                ch_log_error("Could not open file \"%s\". Error returned is \"%s\", reverting to stderr\n",
                                filename,
                                strerror(errno));
                ch_perf.fd = STDERR_FILENO;
            }
            break;
        }
        case ch_perf_output_tostderr:{
            ch_perf.fd = STDERR_FILENO;
            break;
        }
        case ch_perf_output_tostdout:{
            ch_perf.fd = STDOUT_FILENO;
            break;
        }

    }
}


// CH PERF LOG Binary Format
// 0    "CHPLVVV\0" -- MP6PL, followed by version number in XX.X format, followed by a null.
// 8    Number of events in this file
// 16   NUmber of events seen -- This is in case the perf logger runs out of space, we can still know that events happened
// 24   Record 0 - Cycles Stamp - 64bits RDTSC counter value
// 32   Record 0 - Event ID
// 36   Record 0 - Condition ID
// 40   Record 1 - Cycles Stamp
// ...

static int ch_perf_write_header_binary(){
    //Write out a 7 char head + 1 null char
    int result = write(ch_perf.fd,"CHPERF1", strlen("CHPL010") + 1); //"CH Perf Logger version 1.0
    result += write(ch_perf.fd, &ch_perf.event_index, sizeof(u64));
    result += write(ch_perf.fd, &ch_perf.event_count, sizeof(u64));
    return result;
}

//Simple write out the raw binary event
static int ch_perf_write_event_binary(const ch_perf_event_t* event){
    return write(ch_perf.fd, event, sizeof(ch_perf_event_t));
}


static int ch_perf_write_event_ascii(const ch_perf_event_t* event, ch_bool use_csv)
{
    const char sep = use_csv ? ',' : ' ';
    const char start_stop = event->event_id >= (1ULL<<31) ? 'O' : 'A'; //"stArt", "stOp"
    const uint64_t event_id = event->event_id >= (1ULL<<31) ? event->event_id & ~(1<<31) : event->event_id;
    return dprintf(ch_perf.fd,"%c%c%lu%c%u%c%lu\n",  start_stop, sep, event_id, sep, event->cond_id, sep, event->ts);
}


static int ch_perf_write_header_csv(){
    const char* header = "Event Type, Event ID, Condition ID, TSC\n";
    return dprintf(ch_perf.fd,header,"%s",header);
}


static int ch_perf_write_event_csv(const ch_perf_event_t* event){
    return ch_perf_write_event_ascii(event,true);
}


static int ch_perf_write_header_ssv(){
    return 0; //No header for ssv!
}


static int ch_perf_write_event_ssv(const ch_perf_event_t* event){
    return ch_perf_write_event_ascii(event, false);
}


int ch_perf_write_header(ch_perf_format_e format, ch_perf_output_e output)
{
    if(output != ch_perf_output_tofile   &&
       output != ch_perf_output_tostderr &&
       output != ch_perf_output_tostdout ){
        ch_log_fatal("Unknown output type. Giving up!");
    }

    switch(format){
        case ch_perf_format_binary:{
            return ch_perf_write_header_binary();
        }
        case ch_perf_format_csv:{
            return ch_perf_write_header_csv();
        }
        case ch_perf_format_ssv:{
            return ch_perf_write_header_ssv();
        }
        default:
            return -1;
    }

}



int ch_perf_write_event(const ch_perf_event_t* event, ch_perf_format_e format, ch_perf_output_e output)
{
    //This is here in case we add non fd style outputs
    if(output != ch_perf_output_tofile   &&
       output != ch_perf_output_tostderr &&
       output != ch_perf_output_tostdout ){
        ch_log_fatal("Unknown output type. Giving up!");
    }

    switch(format){
        case ch_perf_format_binary:{
            return ch_perf_write_event_binary(event);
        }
        case ch_perf_format_csv:{
            return ch_perf_write_event_csv(event);
        }
        case ch_perf_format_ssv:{
            return ch_perf_write_event_ssv(event);
        }
        default:
            return -1;
    }
}

//Write one last word and close
void ch_perf_close(ch_perf_output_e output)
{
    //This is here in case we add non fd style outputs
    if(output != ch_perf_output_tofile   &&
       output != ch_perf_output_tostderr &&
       output != ch_perf_output_tostdout ){
        ch_log_fatal("Unknown output type. Giving up!");
    }

    close(ch_perf.fd);
}


void ch_perf_finish_(ch_perf_output_e output, ch_perf_format_e format, char* filename){
    if(ch_perf.event_index && ch_perf.max_events){


        ch_perf_open_output(output,filename);
        ch_perf_write_header(format,output);


        for(u64 i = 0; i < MIN(ch_perf.event_index, ch_perf.max_events); i++ ){
            const ch_perf_event_t* event = ch_perf.events + i;
            ch_perf_write_event(event,format,output);
        }

        if(ch_perf.fd != STDERR_FILENO &&
           ch_perf.fd != STDOUT_FILENO ){
            ch_perf_close(output);
        }
    }
}


inline u64 ch_perf_get_watch_ticks()
{
    return ch_perf.timer_stop - ch_perf.timer_start;
}

inline u64 ch_perf_get_tsc_sample()
{
    return ch_perf.tsc;
}

