/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Input stream definition
 *
 */

#ifndef CAMIO_PERF_H_
#define CAMIO_PERF_H_

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>


typedef struct {
    uint64_t ts;             //Time the event was logged
    uint64_t event_id : 32;  //ID of the event, used to tie start and stop operations together
    uint64_t cond_id : 32;   //ID of the event, used to differentiate different start/stop conditions for the same ID.
} camio_perf_event_t;

//Event IDs
enum {
    CAMIO_PERF_EVENT_ISTREAM_BLOB = 0,
    CAMIO_PERF_EVENT_ISTREAM_DAG,
    CAMIO_PERF_EVENT_ISTREAM_LOG,
    CAMIO_PERF_EVENT_ISTREAM_NETMAP,
    CAMIO_PERF_EVENT_ISTREAM_PCAP,
    CAMIO_PERF_EVENT_ISTREAM_PERIODIC_FAST,
    CAMIO_PERF_EVENT_ISTREAM_PERIODIC,
    CAMIO_PERF_EVENT_ISTREAM_RAW,
    CAMIO_PERF_EVENT_ISTREAM_RING,
    CAMIO_PERF_EVENT_ISTREAM_UDP,

    CAMIO_PERF_EVENT_OSTREAM_BLOB,
    CAMIO_PERF_EVENT_OSTREAM_LOG,
    CAMIO_PERF_EVENT_OSTREAM_NETMAP,
    CAMIO_PERF_EVENT_OSTREAM_RAW,
    CAMIO_PERF_EVENT_OSTREAM_RING,
    CAMIO_PERF_EVENT_OSTREAM_UDP,

    CAMIO_PERF_EVENT_IOSTREAM_TCP,
    CAMIO_PERF_EVENT_IOSTREAM_UDP,
    CAMIO_PERF_EVENT_IOSTREAM_SHMEM,

    CAMIO_PERF_EVENT_COUNT
};
#define CAMIO_PERF_EVENT_ID_MAX ( (1 << 31) - 1 )  //Cannot have more than 2 billion event IDs....


//CONDITION IDs
enum{
    CAMIO_PERF_COND_NEW_DATA = 0,
    CAMIO_PERF_COND_EXISTING_DATA,
    CAMIO_PERF_COND_NO_DATA,
    CAMIO_PERF_COND_READ_ERROR,

    CAMIO_PERF_COND_WRITE,
    CAMIO_PERF_COND_WRITE_ASSIGNED,
    CAMIO_PERF_COND_WRITE_ESCAPED,
    CAMIO_PERF_COND_WRITE_ERROR,

    CAMIO_PERF_COND_COUNT

};


typedef struct {
    char* output_descr;
    uint64_t event_count;
    uint64_t event_index;
    uint64_t max_events;
    camio_perf_event_t* events;

} camio_perf_t;


camio_perf_t* camio_perf_init(char* output_descr, uint64_t max_events_count);
void camio_perf_finish(camio_perf_t* camio_perf);


//Stolen from linux/arch/x86/include/asm/msr.h
#define EAX_EDX_VAL(low, high)     ((low) | ((uint64_t)(high) << 32))
#define EAX_EDX_RET(low, high)     "=a" (low), "=d" (high)
#define DECLARE_ARGS(low, high)    uint32_t low, high

//Notes:
//- This function is written as a macro to ensure that it is inlined
//- Generally calls to rdtsc are prepended by a call to cpuid. This is done so that the
//  pipeline is flushed and that there is determinism about the moment when rdtsc is called.
//  Flushing the pipeline is an expensive call and not something that we want to do too much
//  on the critical path. For this reason I've decided to trade a little accuracy for
//  better overall performance.
#define camio_perf_event_start(camio_perf, event, cond)                                         \
    if(likely(camio_perf->event_index < camio_perf->max_events)){                               \
        camio_perf->events[camio_perf->event_index].event_id = event;                           \
        camio_perf->events[camio_perf->event_index].cond_id  = cond;                            \
        DECLARE_ARGS(lo, hi);                                                                   \
        asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                            \
        camio_perf->events[camio_perf->event_index].ts       = EAX_EDX_VAL(lo, hi);             \
        camio_perf->event_index++;                                                              \
    }                                                                                           \
    camio_perf->event_count++;


//Notes:
//- This function is written as a macro to ensure that it is inlined
//- Generally calls to rdtsc are prepended by a call to cpuid. This is done so that the
//  pipeline is flushed and that there is determinism about the moment when rdtsc is called.
//  Flushing the pipeline is an expensive call and not something that we want to do too much
//  on the critical path. For this reason I've decided to trade a little accuracy for
//  better overall performance.
#define camio_perf_event_stop(camio_perf, event, cond)                                          \
    if(likely(camio_perf->event_index < camio_perf->max_events)){                               \
        DECLARE_ARGS(lo, hi);                                                                   \
        asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                            \
        camio_perf->events[camio_perf->event_index].ts       = EAX_EDX_VAL(lo, hi);             \
        camio_perf->events[camio_perf->event_index].event_id = event + (1<<31);                 \
        camio_perf->events[camio_perf->event_index].cond_id  = cond;                            \
        camio_perf->event_index++;                                                              \
    }                                                                                           \
    camio_perf->event_count++;






#endif /* CAMIO_PERF_H_ */
