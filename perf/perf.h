/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Input stream definition
 *
 */

#ifndef CH_PERF_H_
#define CH_PERF_H_

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "../log/log.h" //Include this here, so users don't have to
#include "../utils/util.h"

#warning "This is all broken and arch specifc. Need to make the logic and impl separate"

typedef struct {
    uint64_t ts;             //Time the event was logged
    uint32_t event_id;  //ID of the event, used to tie start and stop operations together
    uint32_t cond_id;   //ID of the event, used to differentiate different start/stop conditions for the same ID.
} ch_perf_event_t;


typedef struct {
    //Perf logging control
    u64 event_count;
    u64 event_index;
    u64 max_events;
    ch_perf_event_t* events;

    //Output control
    int fd;

    //Stop watch temps
    u64 timer_start;
    u64 timer_stop;

    //Get tsc
    u64 tsc;


} ch_perf_t;


#define USE_CH_PERF(events_max)                     \
    ch_perf_event_t ch_perf_events[events_max];     \
    ch_perf_t ch_perf = {                           \
        .event_count   = 0,                         \
        .event_index   = 0,                         \
        .max_events    = events_max,                \
        .events        = ch_perf_events,            \
        .fd            = -1,                        \
        .timer_start   = ~0,                        \
        .timer_stop    = ~0,                        \
        .tsc           = ~0                         \
    }

extern ch_perf_t ch_perf;

//Borrowed from linux/arch/x86/include/asm/msr.h
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
#ifndef NDEBUG
        #define ch_perf_event_start(ch_word_event_id, ch_word_cond_id)                       \
        if(likely(ch_perf.event_index < ch_perf.max_events)){                                \
            ch_perf.events[ch_perf.event_index].event_id = (ch_word_event_id);               \
            ch_perf.events[ch_perf.event_index].cond_id  = (ch_word_cond_id);                \
            DECLARE_ARGS(lo, hi);                                                            \
            asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                     \
            ch_perf.events[ch_perf.event_index].ts       = EAX_EDX_VAL(lo, hi);              \
            ch_perf.event_index++;                                                           \
        }                                                                                    \
        ch_perf.event_count++;
#else
    #define ch_perf_event_start(ch_word_event_id, ch_word_cond_id)
#endif



//Notes:
//- This function is written as a macro to ensure that it is inlined
//- Generally calls to rdtsc are prepended by a call to cpuid. This is done so that the
//  pipeline is flushed and that there is determinism about the moment when rdtsc is called.
//  Flushing the pipeline is an expensive call and not something that we want to do too much
//  on the critical path. For this reason I've decided to trade a little accuracy for
//  better overall performance.
#ifndef NDEBUG
    #define ch_perf_event_stop(ch_word_event_id, ch_word_cond_id)                          \
        if(likely(ch_perf.event_index < ch_perf.max_events)){                              \
            DECLARE_ARGS(lo, hi);                                                          \
            asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                   \
            ch_perf.events[ch_perf.event_index].ts       = EAX_EDX_VAL(lo, hi);            \
            ch_perf.events[ch_perf.event_index].event_id = (ch_word_event_id) | (1<<31);   \
            ch_perf.events[ch_perf.event_index].cond_id  = (ch_word_cond_id);              \
            ch_perf.event_index++;                                                         \
        }                                                                                  \
        ch_perf.event_count++;
#else
    #define ch_perf_event_stop(ch_word_event_id, ch_word_cond_id)
#endif

typedef enum {
    ch_perf_output_tostdout,
    ch_perf_output_tostderr,
    ch_perf_output_tofile,
    //ch_perf_http, -- Not yet implemented
} ch_perf_output_e;


typedef enum {
    ch_perf_format_csv,    //Output CSV -- Easy to read text format.
    ch_perf_format_ssv,    //Output SSV -- Easy to read inside GNU plot, useless for everything else
    ch_perf_format_binary, //Output in binary format -- Nice and compact, easy to store, anaylse later.
    //ch_perf_format_http, -- Not yet implemented
} ch_perf_format_e;


void ch_perf_finish_(ch_perf_output_e output, ch_perf_format_e format, char* filename);

#ifndef NDEBU
#define ch_perf_finish(ch_perf_output, ch_perf_format, filename)\
    ch_perf_finish_(ch_perf_output, ch_perf_format, filename);
#else
    #define ch_perf_finish(ch_perf_output, ch_perf_format, filename)
#endif


#define ch_perf_timer_start {                                                      \
    DECLARE_ARGS(lo, hi);                                                          \
    __asm__ __volatile__("rdtsc" : EAX_EDX_RET(lo, hi));                           \
    ch_perf.timer_start = EAX_EDX_VAL(lo, hi);                                     \
}


#define ch_perf_timer_stop {                                                       \
    DECLARE_ARGS(lo, hi);                                                          \
    asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                   \
    ch_perf.timer_stop = EAX_EDX_VAL(lo, hi);                                      \
}

#define ch_perf_timer_reset {                                                      \
    ch_perf.timer_start = ~0;                                                      \
    ch_perf.timer_stop = ~0;                                                       \
}


#define ch_perf_sample_tsc {                                                       \
    DECLARE_ARGS(lo, hi);                                                          \
    __asm__ __volatile__("rdtsc" : EAX_EDX_RET(lo, hi));                                   \
    ch_perf.tsc = EAX_EDX_VAL(lo, hi);                                             \
}

u64 ch_perf_get_watch_ticks();
u64 ch_perf_get_tsc_sample();


#endif /* CH_PERF_H_ */
