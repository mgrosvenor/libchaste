/*
 * test_logger.c
 *
 *  Created on: Jul 2, 2013
 *      Author: mgrosvenor
 */

//Customize the logging build level
#include "../log/log_levels.h"
#define M6_LOG_BUILD_LVL M6_LOG_LVL_DEBUG1

//Pull in the logger
#include "../log/log.h"

//Use it
USE_M6_LOGGER(true,m6_log_tostderr,"out.log");

int main(int argc, char** argv)
{
    int i = 0;
    for(; i < 10024; i++){ //Loop around a few times. Use valgrind to make sure that there are no memory leaks
        //m6_log_fatal("blah blah%u\n", 43);
        m6_log_error("blah blah%u\n", 42);
        m6_log_warn("blah blah%u\n", 44);
        m6_log_info("blah blah%u\n", 42);
        m6_log_debug1("blah blah%u\n", 42);
        m6_log_debug2("blah blah%u\n", 47);
        m6_log_debug3("blah blah%u\n", 42);
    }
    return 0;
}
