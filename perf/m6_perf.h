/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Input stream definition
 *
 */

#ifndef M6_PERF_H_
#define M6_PERF_H_

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "../log/log.h" //Include this here, so users don't have to
#include "../utils/m6_util.h"

typedef struct {
    uint64_t ts;             //Time the event was logged
    uint64_t event_id : 32;  //ID of the event, used to tie start and stop operations together
    uint64_t cond_id : 32;   //ID of the event, used to differentiate different start/stop conditions for the same ID.
} m6_perf_event_t;


typedef struct {
    //Perf logging control
    u64 event_count;
    u64 event_index;
    u64 max_events;
    m6_perf_event_t* events;

    //Output control
    int fd;

    //Stop watch temps
    u64 timer_start;
    u64 timer_stop;

    //Get tsc
    u64 tsc;


} m6_perf_t;


#define USE_M6_PERF(events_max)                     \
    m6_perf_event_t m6_perf_events[events_max];     \
    m6_perf_t m6_perf = {                           \
        .event_count   = 0,                         \
        .event_index   = 0,                         \
        .max_events    = events_max,                \
        .events        = m6_perf_events,            \
        .fd            = -1,                        \
        .timer_start   = ~0,                        \
        .timer_stop    = ~0,                        \
        .tsc           = ~0                         \
    }

extern m6_perf_t m6_perf;

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
        #define m6_perf_event_start(m6_word_event_id, m6_word_cond_id)                       \
        if(likely(m6_perf.event_index < m6_perf.max_events)){                                \
            m6_perf.events[m6_perf.event_index].event_id = (m6_word_event_id);               \
            m6_perf.events[m6_perf.event_index].cond_id  = (m6_word_cond_id);                \
            DECLARE_ARGS(lo, hi);                                                            \
            asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                     \
            m6_perf.events[m6_perf.event_index].ts       = EAX_EDX_VAL(lo, hi);              \
            m6_perf.event_index++;                                                           \
        }                                                                                    \
        m6_perf.event_count++;
#else
    #define m6_perf_event_start(m6_word_event_id, m6_word_cond_id)
#endif



//Notes:
//- This function is written as a macro to ensure that it is inlined
//- Generally calls to rdtsc are prepended by a call to cpuid. This is done so that the
//  pipeline is flushed and that there is determinism about the moment when rdtsc is called.
//  Flushing the pipeline is an expensive call and not something that we want to do too much
//  on the critical path. For this reason I've decided to trade a little accuracy for
//  better overall performance.
#ifndef NDEBUG
    #define m6_perf_event_stop(m6_word_event_id, m6_word_cond_id)                          \
        if(likely(m6_perf.event_index < m6_perf.max_events)){                              \
            DECLARE_ARGS(lo, hi);                                                          \
            asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                   \
            m6_perf.events[m6_perf.event_index].ts       = EAX_EDX_VAL(lo, hi);            \
            m6_perf.events[m6_perf.event_index].event_id = (m6_word_event_id) | (1<<31);   \
            m6_perf.events[m6_perf.event_index].cond_id  = (m6_word_cond_id);              \
            m6_perf.event_index++;                                                         \
        }                                                                                  \
        m6_perf.event_count++;
#else
    #define m6_perf_event_stop(m6_word_event_id, m6_word_cond_id)
#endif

typedef enum {
    m6_perf_output_tostdout,
    m6_perf_output_tostderr,
    m6_perf_output_tofile,
    //m6_perf_http, -- Not yet implemented
} m6_perf_output_e;


typedef enum {
    m6_perf_format_csv,    //Output CSV -- Easy to read text format.
    m6_perf_format_ssv,    //Output SSV -- Easy to read inside GNU plot, useless for everything else
    m6_perf_format_binary, //Output in binary format -- Nice and compact, easy to store, anaylse later.
    //m6_perf_format_http, -- Not yet implemented
} m6_perf_format_e;


void m6_perf_finish_(m6_perf_output_e output, m6_perf_format_e format, char* filename);

#ifndef NDEBU
#define m6_perf_finish(m6_perf_output, m6_perf_format, filename)\
    m6_perf_finish_(m6_perf_output, m6_perf_format, filename);
#else
    #define m6_perf_finish(m6_perf_output, m6_perf_format, filename)
#endif


#define m6_perf_timer_start {                                                      \
    DECLARE_ARGS(lo, hi);                                                          \
    asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                   \
    m6_perf.timer_start = EAX_EDX_VAL(lo, hi);                                     \
}


#define m6_perf_timer_stop {                                                       \
    DECLARE_ARGS(lo, hi);                                                          \
    asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                   \
    m6_perf.timer_stop = EAX_EDX_VAL(lo, hi);                                      \
}

#define m6_perf_timer_reset {                                                      \
    m6_perf.timer_start = ~0;                                                      \
    m6_perf.timer_stop = ~0;                                                       \
}


#define m6_perf_sample_tsc {                                                       \
    DECLARE_ARGS(lo, hi);                                                          \
    asm volatile("rdtsc" : EAX_EDX_RET(lo, hi));                                   \
    m6_perf.tsc = EAX_EDX_VAL(lo, hi);                                             \
}

inline u64 m6_perf_get_watch_ticks();
inline u64 m6_perf_get_tsc_sample();


#endif /* M6_PERF_H_ */
