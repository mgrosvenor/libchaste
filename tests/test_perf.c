/*
 * test_perf.c
 *
 *  Created on: Jul 6, 2013
 *      Author: mgrosvenor
 */


#include "../perf/m6_perf.h"
#include <stdio.h>


USE_M6_LOGGER_DEFAULT;
USE_M6_PERF(4094);

int main(int argc, char** argv)
{
    m6_perf_sample_tsc;
    printf("TSC at start = %lu\n",  m6_perf_get_tsc_sample());
    m6_perf_timer_reset;
    m6_perf_timer_start;
    m6_perf_event_start(0,0);
    int i;
    for(i = 0; i < 2048; i++){
        if(i < 1024){
            m6_perf_event_stop(0,0);
        }
        else{
            m6_perf_event_stop(0,1);
        }
        m6_perf_event_start(0,0);
    }

    m6_perf_finish(m6_perf_output_tostdout,m6_perf_format_csv,NULL);
    m6_perf_timer_stop;
    printf("Timer diff=%lu\n", m6_perf_get_watch_ticks());

    m6_perf_sample_tsc;
    printf("tsc at end = %lu\n",  m6_perf_get_tsc_sample());

    return 0;
}
