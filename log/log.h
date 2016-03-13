/*
 * log.h
 *
 *  Created on: Jul 2, 2013
 *      Author: mgrosvenor
 */

#include "../types/types.h"
#include "../term_color/term_color.h"
#include "log_levels.h"

#ifndef CH_LOG_H_
#define CH_LOG_H_


ch_word _ch_log_out_(ch_word this_log_lvll, ch_word line_num, const char* filename, const char* format, ... );
ch_word _ch_log_out_va_(ch_word this_log_lvll, ch_word line_num, const char* filename, const char* format, va_list args)
 __attribute__((format(printf, 3, 0)));


#ifndef CH_LOG_BUILD_LVL
    #ifdef NDEBUG
        #define CH_LOG_BUILD_LVL   CH_LOG_LVL_INFO    //Output binary to be capable up to "info" level release mode
    #else
        #define CH_LOG_BUILD_LVL   CH_LOG_LVL_DEBUG3  //Output binary to be capable up to "debug3" level debug mode
    #endif
#endif



//Helper macros to make C99 VAR_ARGS work properly. Yuck! Hacky!
#define ch_log_fatal( /*format, args*/...)  ch_log_fatal_helper(__VA_ARGS__, "")
#define ch_log_error( /*format, args*/...)  ch_log_error_helper(__VA_ARGS__, "")
#define ch_log_warn( /*format, args*/...)   ch_log_warn_helper(__VA_ARGS__, "")
#define ch_log_info( /*format, args*/...)   ch_log_info_helper(__VA_ARGS__, "")
#define ch_log_debug1( /*format, args*/...) ch_log_debug1_helper(__VA_ARGS__, "")
#define ch_log_debug2( /*format, args*/...) ch_log_debug2_helper(__VA_ARGS__, "")
#define ch_log_debug3( /*format, args*/...) ch_log_debug3_helper(__VA_ARGS__, "")

#if 1 //Always defined
    #define ch_log_fatal_helper(format, ...) _ch_log_out_(CH_LOG_LVL_FATAL, __LINE__, __FILE__, format, __VA_ARGS__, "" )
    #define ch_log_fatal_va( format, va_list) _ch_log_out_va_(CH_LOG_LVL_FATAL, __LINE__, __FILE__, format, va_list )
#else
    #define ch_log_fatal_helper( format, ...)
    #define ch_log_fatal_va(format, va_list)
#endif


#if CH_LOG_BUILD_LVL >= CH_LOG_LVL_ERROR
    #define ch_log_error_helper(format, ...) _ch_log_out_(CH_LOG_LVL_ERROR, __LINE__, __FILE__, format, __VA_ARGS__, "" )
    #define ch_log_error_va(format, va_list) _ch_log_out_va_(CH_LOG_LVL_ERROR, __LINE__, __FILE__, format, va_list )
#else
    #define ch_log_error_helper(format, ...)
    #define ch_log_error_va(format, va_list)
#endif


#if CH_LOG_BUILD_LVL >= CH_LOG_LVL_WARN
    #define ch_log_warn_helper(format, ...) _ch_log_out_(CH_LOG_LVL_WARN, __LINE__, __FILE__, format, __VA_ARGS__, "" )
    #define ch_log_warn_va(format, va_list) _ch_log_out_va_(CH_LOG_LVL_WARN, __LINE__, __FILE__, format, va_list )
#else
    #define ch_log_warn_helper(format, ...)
    #define ch_log_warn_va(format, va_list)
#endif

#if CH_LOG_BUILD_LVL >= CH_LOG_LVL_INFO
    #define ch_log_info_helper(format, ...) _ch_log_out_(CH_LOG_LVL_INFO, __LINE__, __FILE__, format, __VA_ARGS__, "" )
    #define ch_log_info_va(format, va_list) _ch_log_out_va_(CH_LOG_LVL_INFO, __LINE__, __FILE__, format, va_list )
#else
    #define ch_log_info_helper(format, ...)
    #define ch_log_info_va(format, va_list)
#endif


#if CH_LOG_BUILD_LVL >= CH_LOG_LVL_DEBUG1
    #define ch_log_debug1_helper(format, ...) _ch_log_out_(CH_LOG_LVL_DEBUG1, __LINE__, __FILE__, format, __VA_ARGS__, "" )
    #define ch_log_debug1_va(format, va_list) _ch_log_out_va_(CH_LOG_LVL_DEBUG1, __LINE__, __FILE__, format, va_list )
#else
    #define ch_log_debug1_helper(format, ...)
    #define ch_log_debug1_va(format, va_list)
#endif

#if CH_LOG_BUILD_LVL >= CH_LOG_LVL_DEBUG2
    #define ch_log_debug2_helper(format, ...) _ch_log_out_(CH_LOG_LVL_DEBUG2, __LINE__, __FILE__, format, __VA_ARGS__ , "" )
    #define ch_log_debug2_va(format, va_list) _ch_log_out_va_(CH_LOG_LVL_DEBUG2, __LINE__, __FILE__, format, va_list )
#else
    #define ch_log_debug2_helper(format, ...)
    #define ch_log_debug2_va(format, va_list)
#endif

#if CH_LOG_BUILD_LVL >= CH_LOG_LVL_DEBUG3
    #define ch_log_debug3_helper(format, ...) _ch_log_out_(CH_LOG_LVL_DEBUG3, __LINE__, __FILE__, format, __VA_ARGS__ , "" )
    #define ch_log_debug3_va(format, va_list) _ch_log_out_va_(CH_LOG_LVL_DEBUG3, __LINE__, __FILE__, format, va_list )
#else
    #define ch_log_debug3_helper(format, ...)
    #define ch_log_debug3_va(format, va_list)
#endif

typedef struct {
    ch_colour_t color;  //Color for the log level
    ch_bool source;     //Include the source filename and line
    ch_bool timestamp;  //Include the timestamp
    char* text;         //Short text such as "Warning" for each instance of this level
} ch_log_level_setting_t;


typedef enum {
    ch_log_tostdout,
    ch_log_tostderr,
    ch_log_tofile,
    ch_log_tosyslog,
    //ch_log_http, -- Not yet implemented
} ch_log_output_e;

typedef struct {
    ch_word log_level;              //Current log level for contorling verbosity/output
    ch_bool use_color;              //Use colors when printing to a terminal
    ch_log_output_e output_mode;    //Where to output to
    char* filename;                 //If outputing to a file, the file name, otherwise ignored
    ch_bool use_utc;                //Use UTC time, otherwise use local time
    ch_bool incl_timezone;          //Include the UTC timezone offset
    ch_word subsec_digits;          //How many sub-second digits to use
    ch_log_level_setting_t lvl_config[CH_LOG_LVL_COUNT]; //Setting for each log level
    int fd;                         //Private, the file descriptor of the output
} ch_log_settings_t;


//Only use this one in your code!
//You can customize most of these settings after calling this "function" to initialize the defaults
//DON'T CHANGE THIS HEADER DIRECTLY!!
#define USE_CH_LOGGER(word_level,bool_use_color,enum_output_mode,str_filename) \
ch_log_settings_t ch_log_settings = { \
    .log_level      = word_level, \
    .use_color      = bool_use_color, \
    .output_mode    = enum_output_mode, \
    .filename       = str_filename, \
    .use_utc        = false, \
    .incl_timezone  = false, \
    .subsec_digits  = 6, \
    .lvl_config  = { \
        { .color = CH_TERM_COL_BRIGHT_RED,      .source = true,  .timestamp = true, .text = "Fatal Error" }, /*FATAL*/\
        { .color = CH_TERM_COL_EMPH_RED,        .source = true, .timestamp = true,  .text = "      Error"       }, /*ERROR*/\
        { .color = CH_TERM_COL_BRIGHT_YELLOW,   .source = true, .timestamp = true,  .text = "    Warning"     }, /*WARNING*/\
        { .color = CH_TERM_COL_BRIGHT_GREEN,    .source = true, .timestamp = true,  .text = "       Info"        }, /*INFO*/\
        { .color = CH_TERM_COL_NONE,            .source = true, .timestamp = true,  .text = "    Debug 1"       }, /*DEBUG 1*/\
        { .color = CH_TERM_COL_NONE,            .source = true,  .timestamp = true, .text = "    Debug 2"       }, /*DEBUG 2*/\
        { .color = CH_TERM_COL_NONE,            .source = true,  .timestamp = true, .text = "    Debug 3"          }  /*DEBUG 3*/\
    }, \
    .fd = -1,  /*This is private, please don't play with it*/\
}


//Simple, unobtrusive settings that any libch components can use.
#define USE_CH_LOGGER_DEFAULT \
ch_log_settings_t ch_log_settings = { \
    .log_level      = CH_LOG_LVL_WARN, \
    .use_color      = false, \
    .output_mode    = CH_LOG_OUT_STDERR, \
    .filename       = "", \
    .use_utc        = false, \
    .incl_timezone  = false, \
    .subsec_digits  = 0, \
    .lvl_config  = { \
        { .color = CH_TERM_COL_NONE, .source = true,  .timestamp = false, .text = "Fatal Error" }, /*FATAL*/\
        { .color = CH_TERM_COL_NONE, .source = false, .timestamp = false, .text = "Error"       }, /*ERROR*/\
        { .color = CH_TERM_COL_NONE, .source = false, .timestamp = false, .text = "Warning"     }, /*WARNING*/\
        { .color = CH_TERM_COL_NONE, .source = false, .timestamp = false, .text = "Info"        }, /*INFO*/\
        { .color = CH_TERM_COL_NONE, .source = false, .timestamp = false, .text = "Debug"       }, /*DEBUG 1*/\
        { .color = CH_TERM_COL_NONE, .source = false, .timestamp = false, .text = "Debug"       }, /*DEBUG 2*/\
        { .color = CH_TERM_COL_NONE, .source = false, .timestamp = false, .text = "Debug"       }  /*DEBUG 3*/\
    }, \
    .fd = -1,  /*This is private, please don't play with it*/\
}




#endif /* CH_LOG_H_ */
