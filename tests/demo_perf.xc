/*
 * test_perf.c
 *
 *  Created on: Jul 6, 2013
 *      Author: mgrosvenor
 */


#include "../perf/perf.h"
#include <stdio.h>


USE_CH_LOGGER_DEFAULT;
USE_CH_PERF(4094);

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    ch_perf_sample_tsc;
    printf("TSC at start = %llu\n",  ch_perf_get_tsc_sample());
    ch_perf_timer_reset;
    ch_perf_timer_start;
    ch_perf_event_start(0,0);
    int i;
    for(i = 0; i < 2048; i++){
        if(i < 1024){
            ch_perf_event_stop(0,0);
        }
        else{
            ch_perf_event_stop(0,1);
        }
        ch_perf_event_start(0,0);
    }

    ch_perf_finish(ch_perf_output_tostdout,ch_perf_format_csv,NULL);
    ch_perf_timer_stop;
    printf("Timer diff=%llu\n", ch_perf_get_watch_ticks());

    ch_perf_sample_tsc;
    printf("tsc at end = %llu\n",  ch_perf_get_tsc_sample());

    return 0;
}
