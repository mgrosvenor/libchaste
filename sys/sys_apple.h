/*
 * sys_apple.h
 *
 *  Created on: Apr 23, 2015
 *      Author: mgrosvenor
 */

#ifndef SYS_APPLE_H_
#define SYS_APPLE_H_

#include <time.h>
#include <sys/time.h>

#ifdef __APPLE__ //These definitions are only required for apple/darwin

#define CLOCK_REALTIME 0
void clock_gettime(int ignored, struct timespec *ts);

#endif

#endif /* SYS_APPLE_H_ */
