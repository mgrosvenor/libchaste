/*
 * test_perf.c
 *
 *  Created on: Jul 6, 2013
 *      Author: mgrosvenor
 */


#include "../perf/m6_perf.h"


USE_M6_LOGGER_DEFAULT;
USE_M6_PERF(4094);

int main(int argc, char** argv)
{
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

    return 0;
}
