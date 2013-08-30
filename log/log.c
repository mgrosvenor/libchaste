#include "log.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>

//#include "../deps/libbstring/bstraux.h"
#include "../timing/timestamp.h"
#include "../term_color/term_color.h"


//Suck in the log settings structure from "somewhere"...
extern m6_log_settings_t m6_log_settings;


//Convert from M6 log levels to syslog levels
const char m6_log_to_syslog_lvls[M6_LOG_LVL_COUNT] = {
        /*FATAL*/       LOG_CRIT,
        /*ERROR*/       LOG_ERR,
        /*WARNING*/     LOG_WARNING,
        /*INFO*/        LOG_INFO,
        /*DEBUG 1*/     LOG_DEBUG,
        /*DEBUG 2*/     LOG_DEBUG,
        /*DEBUG 3*/     LOG_DEBUG,
};


m6_word _m6_log_out_va_write(m6_word level, const bstring format, va_list args)
{
    for(;;){
        switch(m6_log_settings.output_mode){
            case m6_log_tostdout:{
                if(m6_log_settings.fd == -1){
                    m6_log_settings.fd = STDOUT_FILENO;
                }
            }  //Fall through to next case is intentional


            case m6_log_tostderr:{
                if(m6_log_settings.fd == -1){
                    m6_log_settings.fd = STDERR_FILENO;
                }
            } //Fall through to next case is intentional


            case m6_log_tofile:{
                if(m6_log_settings.fd == -1){

                    if(m6_log_settings.filename){ //Only open the file if it's a string
                        m6_log_settings.fd = open(m6_log_settings.filename, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
                    }

                    if(m6_log_settings.fd == -1 ){
                        m6_str timestamp = generate_iso_timestamp(m6_log_settings.use_utc,m6_log_settings.subsec_digits, m6_log_settings.incl_timezone);
                        dprintf(STDERR_FILENO, "[%s][Error][%s:%u]: Could not open file \"%s\". Error returned is \"%s\", reverting to stderr\n",
                                cstr(timestamp),
                                __FILE__,
                                __LINE__,
                                m6_log_settings.filename,
                                strerror(errno));
                        bstrFree(timestamp);
                        m6_log_settings.fd = STDERR_FILENO;
                    }
                }


                //Finally write out the value, this is used for the STDOUT, STDERR and FILE cases above
                if(m6_log_settings.use_color && m6_log_settings.output_mode != m6_log_tofile){ //Can happen when falling through from above cases
                    return dprintf_color_va(m6_log_settings.fd,m6_log_settings.lvl_config[level].color,format,args);
                }
                else{
                    return vdprintf(m6_log_settings.fd,cstr(format),args);
                }


                break;

            }


            case m6_log_tosyslog:{
                if(m6_log_settings.fd == -1){
                    openlog(NULL,LOG_CONS | LOG_PID, LOG_USER);
                    m6_log_settings.fd = 0; //Dummy this in so we don't init everytime
                }

                vsyslog(m6_log_to_syslog_lvls[level],cstr(format),args);
                return 0;
            }


            default:{
                m6_str timestamp = generate_iso_timestamp(m6_log_settings.use_utc,m6_log_settings.subsec_digits, m6_log_settings.incl_timezone);
                dprintf(STDERR_FILENO, "[%s][Error][%s:%u]: Unknown log output type \"%u\", reverting to stderr\n",
                        cstr(timestamp),
                        __FILE__,
                        __LINE__,
                        m6_log_settings.output_mode );
                bstrFree(timestamp);
            }
        }
    }

    return -1; //Should never get here!!

}



m6_word _m6_log_out_va_(
        m6_word level,
        m6_word line_num,
        const char* filename,
        const char* format, va_list args )
{
    if(level > m6_log_settings.log_level){
        return 0; //Early exit for logs we don't want
    }

    m6_str final_format = bfromcstralloc(512, ""); //This should keep reallocs to a minimum

    if(m6_log_settings.lvl_config[level].timestamp){
        bconchar(final_format,'[');
        m6_str timestamp = generate_iso_timestamp(m6_log_settings.use_utc,m6_log_settings.subsec_digits, m6_log_settings.incl_timezone);
        bconcat(final_format,timestamp);
        bstrFree(timestamp);
        bconchar(final_format,']');
    }

    if(m6_log_settings.lvl_config[level].text){
        bformata(final_format, "[%s]", m6_log_settings.lvl_config[level].text);
    }

    if(m6_log_settings.lvl_config[level].source){
        bformata(final_format, "(%s:%u)", filename, line_num);
    }



    if(m6_log_settings.lvl_config[level].source      ||
       m6_log_settings.lvl_config[level].timestamp   ||
       m6_log_settings.lvl_config[level].text        ){
        bcatcstr(final_format,": ");
    }

    bcatcstr(final_format, format);

    //Do the final write out
    _m6_log_out_va_write(level,final_format,args);

    bstrFree(final_format);

    //Make fatal logs, actually fatal
    if(level == M6_LOG_LVL_FATAL){
        exit(-1);
    }

    return 0;

}


m6_word _m6_log_out_(
        m6_word level,
        m6_word line_num,
        const char* filename,
        const char* format, ... ) //Intentionally using char* here as these are passed in as constants
{

    va_list args;
    va_start(args,format);
    m6_word result = _m6_log_out_va_(level,line_num, filename,format,args);
    va_end(args);

    return result;
}
