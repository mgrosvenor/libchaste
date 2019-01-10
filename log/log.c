#include "log.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>

#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */


#include <unistd.h>
#include <fcntl.h>

#include "../types/types.h"
#include "../timing/timestamp.h"
#include "../term_color/term_color.h"
#include "../string/string.h"
#include "../log/log.h"


//Suck in the log settings structure from "somewhere"...
extern ch_log_settings_t ch_log_settings;


//Convert from CH log levels to syslog levels
const char ch_log_to_syslog_lvls[CH_LOG_LVL_COUNT] = {
        /*FATAL*/       LOG_CRIT,
        /*ERROR*/       LOG_ERR,
        /*WARNING*/     LOG_WARNING,
        /*INFO*/        LOG_INFO,
        /*DEBUG 1*/     LOG_DEBUG,
        /*DEBUG 2*/     LOG_DEBUG,
        /*DEBUG 3*/     LOG_DEBUG,
};


ch_word _ch_elog_out_va_write(ch_log_settings_t* settings,
                              ch_word level, const ch_str format, va_list args)
{
    for(;;){
        switch(settings->output_mode){
            case ch_log_tostdout:{
                if(settings->fd == -1){
                    settings->fd = STDOUT_FILENO;
                }
            }  //Fall through to next case is intentional


            case ch_log_tostderr:{
                if(settings->fd == -1){
                    settings->fd = STDERR_FILENO;
                }
            } //Fall through to next case is intentional


            case ch_log_tofile:{
                if(settings->fd == -1){

                    if(settings->filename){ //Only open the file if it's a string
                        settings->fd = open(settings->filename, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
                    }

                    if(settings->fd == -1 ){
                        ch_str timestamp = generate_iso_timestamp(settings->use_utc,settings->subsec_digits, settings->incl_timezone);
                        ch_str error_txt = CH_STR("", 128);
                        strerror_r(errno,error_txt.cstr, error_txt.mlen);
                        dprintf(STDERR_FILENO, "[%s][Error][%s:%u]: Could not open file \"%s\". Error returned is \"%s\", reverting to stderr\n",
                                timestamp.cstr,
                                __FILE__,
                                __LINE__,
                                settings->filename,
                                error_txt.cstr
                                );
                        ch_str_free(&timestamp);
                        ch_str_free(&error_txt);
                        settings->fd = STDERR_FILENO;
                    }
                }


                //Finally write out the value, this is used for the STDOUT, STDERR and FILE cases above
                if(settings->use_color && settings->output_mode != ch_log_tofile){ //Can happen when falling through from above cases
                    return dprintf_color_va(settings->fd,settings->lvl_config[level].color,format,args);
                }
                else{
                    return vdprintf(settings->fd,CH_STR_CSTR(format),args);
                }


                break;

            }


            case ch_log_tosyslog:{
                if(settings->fd == -1){
                    openlog(NULL,LOG_CONS | LOG_PID, LOG_USER);
                    settings->fd = 0; //Dummy this in so we don't init everytime
                }

                vsyslog(ch_log_to_syslog_lvls[level],CH_STR_CSTR(format),args);
                return 0;
            }


            default:{
                ch_str timestamp = generate_iso_timestamp(settings->use_utc,settings->subsec_digits, settings->incl_timezone);
                dprintf(STDERR_FILENO, "[%s][Error][%s:%u]: Unknown log output type \"%u\", reverting to stderr\n",
                        CH_STR_CSTR(timestamp),
                        __FILE__,
                        __LINE__,
                        settings->output_mode );
                ch_str_free(&timestamp);
            }
        }
    }

    return -1; //Should never get here!!

}



ch_word _ch_elog_out_va_(
        ch_log_settings_t* settings,
        ch_word level,
        ch_word line_num,
        const char* filename,
        const char* format, va_list args )
{
    if(level > settings->log_level){
        return 0; //Early exit for logs we don't want
    }

    ch_str final_format = CH_STR("", 512); //This should keep reallocs to a minimum

    if(settings->lvl_config[level].timestamp){
        CH_STR_CAT_CHAR(&final_format,'[');
        ch_str timestamp = generate_iso_timestamp(settings->use_utc,settings->subsec_digits, settings->incl_timezone);
        CH_STR_CAT(&final_format,timestamp);
        ch_str_free(&timestamp);
        CH_STR_CAT_CHAR(&final_format,']');
    }

    if(settings->lvl_config[level].pid){
        pid_t tid;
        tid = syscall(SYS_gettid);
        pid_t pid = getpid();
        CH_STR_LEN(final_format) += snprintf(CH_STR_CSTR_END(final_format), CH_STR_AVAIL(final_format), "[%5i][%5i]", pid, tid);
    }

    if(settings->lvl_config[level].text){
        CH_STR_LEN(final_format) += snprintf(CH_STR_CSTR_END(final_format), CH_STR_AVAIL(final_format), "[%s]",settings->lvl_config[level].text );
    }

    if(settings->lvl_config[level].source){
        CH_STR_LEN(final_format) += snprintf( CH_STR_CSTR_END(final_format), CH_STR_AVAIL(final_format), "(%s:%lli)", filename, line_num);
    }

    if(settings->lvl_config[level].source      ||
       settings->lvl_config[level].timestamp   ||
       settings->lvl_config[level].text        ){
        CH_STR_CAT_CSTR(&final_format,": ");
    }

    CH_STR_CAT_CSTR(&final_format, format);

    //Do the final write out
    _ch_elog_out_va_write(settings, level,final_format,args);

    ch_str_free(&final_format);

    //Make fatal logs, actually fatal
    if(level == CH_LOG_LVL_FATAL){
        exit(-1);
    }

    return 0;

}


ch_word _ch_log_out_va_(
        ch_word level,
        ch_word line_num,
        const char* filename,
        const char* format, va_list args )
{
    return _ch_elog_out_va_(&ch_log_settings, level, line_num, filename, format, args);
}


ch_word _ch_elog_out_(
        ch_log_settings_t* settings,
        ch_word level,
        ch_word line_num,
        const char* filename,
        const char* format, ... ) //Intentionally using char* here as these are passed in as constants
{

    va_list args;
    va_start(args,format);
    ch_word result = _ch_elog_out_va_(settings, level,line_num, filename,format,args);
    va_end(args);

    return result;
}

ch_word _ch_log_out_(
        ch_word level,
        ch_word line_num,
        const char* filename,
        const char* format, ... ) //Intentionally using char* here as these are passed in as constants
{

    va_list args;
    va_start(args,format);
    ch_word result = _ch_elog_out_va_(&ch_log_settings, level,line_num, filename,format,args);
    va_end(args);
    return result;
}
