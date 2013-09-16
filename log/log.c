#include "log.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>

#include "../types/types.h"
#include "../timing/timestamp.h"
#include "../term_color/term_color.h"
#include "../string/string.h"


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


ch_word _ch_log_out_va_write(ch_word level, const ch_str format, va_list args)
{
    for(;;){
        switch(ch_log_settings.output_mode){
            case ch_log_tostdout:{
                if(ch_log_settings.fd == -1){
                    ch_log_settings.fd = STDOUT_FILENO;
                }
            }  //Fall through to next case is intentional


            case ch_log_tostderr:{
                if(ch_log_settings.fd == -1){
                    ch_log_settings.fd = STDERR_FILENO;
                }
            } //Fall through to next case is intentional


            case ch_log_tofile:{
                if(ch_log_settings.fd == -1){

                    if(ch_log_settings.filename){ //Only open the file if it's a string
                        ch_log_settings.fd = open(ch_log_settings.filename, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
                    }

                    if(ch_log_settings.fd == -1 ){
                        ch_str timestamp = generate_iso_timestamp(ch_log_settings.use_utc,ch_log_settings.subsec_digits, ch_log_settings.incl_timezone);
                        ch_str error_txt = CH_STR("", 128);
                        strerror_r(errno,error_txt.cstr, error_txt.mlen);
                        dprintf(STDERR_FILENO, "[%s][Error][%s:%u]: Could not open file \"%s\". Error returned is \"%s\", reverting to stderr\n",
                                timestamp.cstr,
                                __FILE__,
                                __LINE__,
                                ch_log_settings.filename,
                                error_txt.cstr
                                );
                        ch_str_free(&timestamp);
                        ch_str_free(&error_txt);
                        ch_log_settings.fd = STDERR_FILENO;
                    }
                }


                //Finally write out the value, this is used for the STDOUT, STDERR and FILE cases above
                if(ch_log_settings.use_color && ch_log_settings.output_mode != ch_log_tofile){ //Can happen when falling through from above cases
                    return dprintf_color_va(ch_log_settings.fd,ch_log_settings.lvl_config[level].color,format,args);
                }
                else{
                    return vdprintf(ch_log_settings.fd,CH_STR_CSTR(format),args);
                }


                break;

            }


            case ch_log_tosyslog:{
                if(ch_log_settings.fd == -1){
                    openlog(NULL,LOG_CONS | LOG_PID, LOG_USER);
                    ch_log_settings.fd = 0; //Dummy this in so we don't init everytime
                }

                vsyslog(ch_log_to_syslog_lvls[level],CH_STR_CSTR(format),args);
                return 0;
            }


            default:{
                ch_str timestamp = generate_iso_timestamp(ch_log_settings.use_utc,ch_log_settings.subsec_digits, ch_log_settings.incl_timezone);
                dprintf(STDERR_FILENO, "[%s][Error][%s:%u]: Unknown log output type \"%u\", reverting to stderr\n",
                        CH_STR_CSTR(timestamp),
                        __FILE__,
                        __LINE__,
                        ch_log_settings.output_mode );
                ch_str_free(&timestamp);
            }
        }
    }

    return -1; //Should never get here!!

}



ch_word _ch_log_out_va_(
        ch_word level,
        ch_word line_num,
        const char* filename,
        const char* format, va_list args )
{
    if(level > ch_log_settings.log_level){
        return 0; //Early exit for logs we don't want
    }

    ch_str final_format = CH_STR("", 512); //This should keep reallocs to a minimum

    if(ch_log_settings.lvl_config[level].timestamp){
        CH_STR_CAT_CHAR(&final_format,'[');
        ch_str timestamp = generate_iso_timestamp(ch_log_settings.use_utc,ch_log_settings.subsec_digits, ch_log_settings.incl_timezone);
        CH_STR_CAT(&final_format,timestamp);
        ch_str_free(&timestamp);
        CH_STR_CAT_CHAR(&final_format,']');
    }

    if(ch_log_settings.lvl_config[level].text){
        CH_STR_LEN(final_format) += snprintf(CH_STR_CSTR_END(final_format), CH_STR_AVAIL(final_format), "[%s]", ch_log_settings.lvl_config[level].text);
    }

    if(ch_log_settings.lvl_config[level].source){
        CH_STR_LEN(final_format) += snprintf( CH_STR_CSTR_END(final_format), CH_STR_AVAIL(final_format), "(%s:%li)", filename, line_num);
    }



    if(ch_log_settings.lvl_config[level].source      ||
       ch_log_settings.lvl_config[level].timestamp   ||
       ch_log_settings.lvl_config[level].text        ){
        CH_STR_CAT_CSTR(&final_format,": ");
    }

    CH_STR_CAT_CSTR(&final_format, format);

    //Do the final write out
    _ch_log_out_va_write(level,final_format,args);

    ch_str_free(&final_format);

    //Make fatal logs, actually fatal
    if(level == CH_LOG_LVL_FATAL){
        exit(-1);
    }

    return 0;

}


ch_word _ch_log_out_(
        ch_word level,
        ch_word line_num,
        const char* filename,
        const char* format, ... ) //Intentionally using char* here as these are passed in as constants
{

    va_list args;
    va_start(args,format);
    ch_word result = _ch_log_out_va_(level,line_num, filename,format,args);
    va_end(args);

    return result;
}
