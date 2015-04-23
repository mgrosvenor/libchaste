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


#define CLOCK_REALTIME 0
void clock_gettime(int ignored, struct timespec *ts);

#endif /* SYS_APPLE_H_ */
