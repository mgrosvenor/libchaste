/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Input stream factory definition
 *
 */

#include <string.h>

#include "camio_perf.h"
#include "../utils/camio_util.h"
#include "../errors/camio_errors.h"
#include "../ostreams/camio_ostream.h"

camio_perf_t* camio_perf_init(char* output_descr, uint64_t max_events_count){
    if(sizeof(camio_perf_event_t) != 2 * sizeof(uint64_t)){
        eprintf_exit("size of camio_perf_event_t is not a multiple of uin64_t");
    }

    camio_perf_t* result = malloc(sizeof(camio_perf_t));
    if(!result){
        eprintf_exit("Could not allocate memory for camio perf\n");
    }
    bzero(result,sizeof(camio_perf_t));

    result->events = malloc(sizeof(camio_perf_event_t) * max_events_count);
    if(!result){
        eprintf_exit("Could not allocate memory for camio perf events\n");
    }

    bzero(result->events,sizeof(camio_perf_event_t) * max_events_count);
    result->output_descr = output_descr;
    result->max_events   = max_events_count;
    return result;
}

void camio_perf_finish(camio_perf_t* camio_perf){
    if(!camio_perf){
        return;
    }

    if(camio_perf->event_index && camio_perf->max_events){
        char out_buff[1024];
        uint64_t out_len;
        camio_ostream_t* out = camio_ostream_new(camio_perf->output_descr,NULL,NULL, NULL);
        if(!out){
            eprintf_exit("Could not create output stream for camio perf\n");
        }

        out_len = snprintf(out_buff,1024,"F %lu, C %lu", camio_perf->event_count, camio_perf->event_index);
        out->assign_write(out,(uint8_t*)out_buff,out_len);
        out->end_write(out,out_len);
        size_t i = 0;
        for(i = 0; i < MIN(camio_perf->event_index, camio_perf->max_events); i++ ){
            const camio_perf_event_t event = camio_perf->events[i];
            char start_stop = event.event_id > (1<<31) ? 'O' : 'A'; //"stArt", "stOp"
            const uint64_t event_id = event.event_id > (1<<31) ? event.event_id & ~(1<<31) : event.event_id;
            out_len = snprintf(out_buff,1024,"%c, %lu, %lu, %u",  start_stop, event.ts, event_id, event.cond_id);
            out->assign_write(out,(uint8_t*)out_buff,out_len);
            out->end_write(out,out_len);
        }

        out->delete(out);
    }


    free(camio_perf->events);
    free(camio_perf);
}
