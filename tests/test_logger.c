/*
 * test_logger.c
 *
 *  Created on: Jul 2, 2013
 *      Author: mgrosvenor
 */

//Customize the logging build level - This is not necessary and can be omitted if you are happy with the default
//This MUST come before #include "../log/log.h"!
//**************************************************************************************************************
#include "../log/log_levels.h"
#define M6_LOG_BUILD_LVL M6_LOG_LVL_DEBUG1
//**************************************************************************************************************


//Pull in the logger
#include "../log/log.h"

//Use it
USE_M6_LOGGER(M6_LOG_LVL_INFO,true,m6_log_tostdout,"out.log");

int main(int argc, char** argv)
{
    int i = 0;
    for(; i < 1024; i++){ //Loop around a few times. Use valgrind to make sure that there are no memory leaks

        m6_log_error("Error text here - %u\n", i);
        m6_log_warn("Warning text here - %u\n", i);
        m6_log_info("Info text here - %u\n", i);
        m6_log_debug1("Debug text here - %u\n", i);
        m6_log_debug2("Debug 2 text here - %u\n", i);
        m6_log_debug3("Debug 3 text here - %u\n", i);
    }

    m6_log_fatal("Fatal error text here %u\n", i);

    return 0;
}
