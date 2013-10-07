// CamIO 2: perf_mon.h
// Copyright (C) 2013: Matthew P. Grosvenor (matthew.grosvenor@cl.cam.ac.uk) 
// Licensed under BSD 3 Clause, please see LICENSE for more details. 

#ifndef PERF_MON_H_
#define PERF_MON_H_

#include "../types/types.h"
#include <string.h>

/**
 * The TSC counter structure is used for both time and cycle based profiling/performance monitoring.
 * The intention is that 1 or more of these structures will be used in an array. Multiple instances
 * are used to monitor multiple stop conditions. i.e. If there are multiple paths to exit a function,
 * then an idea for which paths are taken most often and how much they cost can be gathered in real-
 * time or compiled out. The strong intention is that performance monitoring should be light weight
 * enough that it can be run in production without affecting performance (too much). At a minimum,
 * counter based performance monitoring is offered which only adds an additional increment operator
 * as it's cost but provides useful insight into code performance
 */

typedef struct {
    //Sample using just incrementing a counter
    u64 start_count; //Number of times start counter was called. Only used for counter 0
    u64 end_count; //Number of times end counter was called

    //Sampling using timestamp counter
    u64 nanos_tmp; //Only used for counter 0
    u64 nanos_total;
    u64 nanos_max;
    u64 nanos_min;
    double nanos_avg;

    //Sampling using cycle counter
    u64 cycles_tmp; //Only used for counter 0
    u64 cycles_total;
    u64 cycles_max;
    u64 cycles_min;
    double cycles_avg;
} TSC;


/**
 * Per module performance monitoring. Use only once per file scope. Never in headers.
 */

struct perf_mod_generic;
typedef struct perf_mod_generic perf_mod_generic_t;

struct perf_mod_generic{
    ch_word id;
    ch_cstr descr;
    ch_word descr_len;
};


#define make_perf_module(PERFS)                 \
    static struct perf_mod_t {                  \
        ch_word id;                             \
        ch_cstr descr;                          \
        ch_word descr_len;                      \
        PERFS                                   \
    } perf_mod = { .id = 0, .descr = #PERFS, . descr_len = strlen(#PERFS) }


#define get_perf_mod_ref (perf_mod_generic_t*)&perf_mod

#define perf_mod_start( NAME ) \
    perf_mod.NAME[0].start_count += 1; \
    perf_mod.NAME[0].nanos_tmp = get_nanos_now();


#define perf_mod_end( NAME, ID ) \
    perf_mod.NAME[ID].end_count     += 1; \
    perf_mod.NAME[0].nanos_tmp      = (get_nanos_now() - perf_mod.NAME[0].nanos_tmp); \
    perf_mod.NAME[ID].nanos_total   += perf_mod.NAME[0].nanos_tmp; \
    perf_mod.NAME[ID].nanos_min     = perf_mod.NAME[ID].nanos_min == 0 ? perf_mod.NAME[0].nanos_tmp : perf_mod.NAME[0].nanos_tmp < perf_mod.NAME[ID].nanos_min ? perf_mod.NAME[0].nanos_tmp : perf_mod.NAME[ID].nanos_min ; \
    perf_mod.NAME[ID].nanos_max     = perf_mod.NAME[0].nanos_tmp > perf_mod.NAME[ID].nanos_max ? perf_mod.NAME[0].nanos_tmp : perf_mod.NAME[ID].nanos_max ;


void print_perf_stats(perf_mod_generic_t* perf_mod);
u64 get_nanos_now();

#endif /* PERF_MON_H_ */
