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
#define CH_LOG_BUILD_LVL CH_LOG_LVL_DEBUG3
//**************************************************************************************************************

//Pull in the logger
#include "../log/log.h"

#include <stdio.h>

//Use it
//
USE_CH_LOGGER(CH_LOG_LVL_INFO,true,ch_log_tostdout,"NULL");

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    int i = 0;
    for(; i < 10; i++){ //Loop around a few times. Use valgrind to make sure that there are no memory leaks

        ch_log_settings.log_level = i;
        printf("Log level = %i\n", i);
        printf("--------------------------------------------\n");
        ch_log_error("Error text here - %u\n", i);
        ch_log_warn("Warning text here - %u\n", i);
        ch_log_debug1("Debug text here - %u\n", i);
        ch_log_debug2("Debug 2 text here - %u\n", i);
        ch_log_debug3("Debug 3 text here - %u\n", i);
        printf("--------------------------------------------\n");

//        ch_log_settings.use_utc = i % 2;
//        ch_log_settings.incl_timezone = i % 3;
//        ch_log_settings.subsec_digits = 10 - i;
//        ch_log_info("Info text here - %u\n", i);
//        ch_log_settings.lvl_config[CH_LOG_LVL_INFO].source =  !ch_log_settings.lvl_config[CH_LOG_LVL_INFO].source;




    }

    //ch_log_fatal("Fatal error text here %u\n", i); This will cause unit tests to fail!

    return 0;
}
