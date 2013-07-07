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

#include "m6_perf.h"
#include "../utils/m6_util.h"
#include "../log/log.h"


void m6_perf_open_output(m6_perf_output_e output,const char* filename)
{
    switch(output){
        case m6_perf_output_tofile: {
            m6_perf.fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
            if(m6_perf.fd < 0){
                m6_log_error("Could not open file \"%s\". Error returned is \"%s\", reverting to stderr\n",
                                filename,
                                strerror(errno));
                m6_perf.fd = STDERR_FILENO;
            }
            break;
        }
        case m6_perf_output_tostderr:{
            m6_perf.fd = STDERR_FILENO;
            break;
        }
        case m6_perf_output_tostdout:{
            m6_perf.fd = STDOUT_FILENO;
            break;
        }

    }
}


// M6 PERF LOG Binary Format
// 0    "M6PLVVV\0" -- MP6PL, followed by version number in XX.X format, followed by a null.
// 8    Number of events in this file
// 16   NUmber of events seen -- This is in case the perf logger runs out of space, we can still know that events happened
// 24   Record 0 - Cycles Stamp - 64bits RDTSC counter value
// 32   Record 0 - Event ID
// 36   Record 0 - Condition ID
// 40   Record 1 - Cycles Stamp
// ...

static int m6_perf_write_header_binary(){
    //Write out a 7 char head + 1 null char
    int result = write(m6_perf.fd,"M6PERF1", strlen("M6PL010") + 1); //"M6 Perf Logger version 1.0
    result += write(m6_perf.fd, &m6_perf.event_index, sizeof(u64));
    result += write(m6_perf.fd, &m6_perf.event_count, sizeof(u64));
    return result;
}

//Simple write out the raw binary event
static int m6_perf_write_event_binary(const m6_perf_event_t* event){
    return write(m6_perf.fd, &m6_perf.event_count, sizeof(m6_perf_event_t));
}


static int m6_perf_write_event_ascii(const m6_perf_event_t* event, m6_bool use_csv)
{
    const char sep = use_csv ? ',' : ' ';
    const char start_stop = event->event_id >= (1<<31) ? 'O' : 'A'; //"stArt", "stOp"
    const uint64_t event_id = event->event_id >= (1<<31) ? event->event_id & ~(1<<31) : event->event_id;
    return dprintf(m6_perf.fd,"%c%c%lu%c%u%c%lu\n",  start_stop, sep, event_id, sep, event->cond_id, sep, event->ts);
}


static int m6_perf_write_header_csv(){
    const char* header = "Event Type, Event ID, Condition ID, TSC\n";
    return dprintf(m6_perf.fd,header,"%s",header);
}


static int m6_perf_write_event_csv(const m6_perf_event_t* event){
    return m6_perf_write_event_ascii(event,true);
}


static int m6_perf_write_header_ssv(){
    return 0; //No header for ssv!
}


static int m6_perf_write_event_ssv(const m6_perf_event_t* event){
    return m6_perf_write_event_ascii(event, false);
}


int m6_perf_write_header(m6_perf_format_e format, m6_perf_output_e output)
{
    if(output != m6_perf_output_tofile   &&
       output != m6_perf_output_tostderr &&
       output != m6_perf_output_tostdout ){
        m6_log_fatal("Unknown output type. Giving up!");
    }

    switch(format){
        case m6_perf_format_binary:{
            return m6_perf_write_header_binary();
        }
        case m6_perf_format_csv:{
            return m6_perf_write_header_csv();
        }
        case m6_perf_format_ssv:{
            return m6_perf_write_header_ssv();
        }
        default:
            return -1;
    }

}



int m6_perf_write_event(const m6_perf_event_t* event, m6_perf_format_e format, m6_perf_output_e output)
{
    //This is here in case we add non fd style outputs
    if(output != m6_perf_output_tofile   &&
       output != m6_perf_output_tostderr &&
       output != m6_perf_output_tostdout ){
        m6_log_fatal("Unknown output type. Giving up!");
    }

    switch(format){
        case m6_perf_format_binary:{
            return m6_perf_write_event_binary(event);
        }
        case m6_perf_format_csv:{
            return m6_perf_write_event_csv(event);
        }
        case m6_perf_format_ssv:{
            return m6_perf_write_event_ssv(event);
        }
        default:
            return -1;
    }
}

//Write one last word and close
void m6_perf_close(m6_perf_output_e output)
{
    //This is here in case we add non fd style outputs
    if(output != m6_perf_output_tofile   &&
       output != m6_perf_output_tostderr &&
       output != m6_perf_output_tostdout ){
        m6_log_fatal("Unknown output type. Giving up!");
    }

    close(m6_perf.fd);
}


void m6_perf_finish_(m6_perf_output_e output, m6_perf_format_e format, char* filename){
    if(m6_perf.event_index && m6_perf.max_events){


        m6_perf_open_output(output,filename);
        m6_perf_write_header(format,output);

        m6_word i = 0;
        for(i = 0; i < MIN(m6_perf.event_index, m6_perf.max_events); i++ ){
            const m6_perf_event_t* event = m6_perf.events + i;
            m6_perf_write_event(event,format,output);
        }

        m6_perf_close(output);
    }
}
