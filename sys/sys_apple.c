/*
 * sys_apple.c
 *
 *  Created on: Apr 23, 2015
 *      Author: mgrosvenor
 */


#include "sys_apple.h"
#include <mach/clock.h>
#include <mach/mach.h>

//Code liberally borrowed from https://gist.github.com/jbenet/1087739
void clock_gettime(int ignored, struct timespec *ts) {
    (void)ignored;

    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
}

