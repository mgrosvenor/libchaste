/*
 * log.h
 *
 *  Created on: Jul 2, 2013
 *      Author: mgrosvenor
 */

#include "../types/types.h"
#include "../term_color/term_color.h"
#include "log_levels.h"

#ifndef M6_LOG_H_
#define M6_LOG_H_


m6_word _m6_log_out_(m6_word this_log_lvll, m6_word line_num, const char* filename, const char* format, ... );
m6_word _m6_log_out_va_(m6_word this_log_lvll, m6_word line_num, const char* filename, const char* format, va_list args);


#ifndef M6_LOG_BUILD_LVL
    #if NDEBUG
        #define M6_LOG_BUILD_LVL   M6_LOG_LVL_INFO    //Output binary to be capable up to "info" level release mode
    #else
        #define M6_LOG_BUILD_LVL   M6_LOG_LVL_DEBUG3  //Output binary to be capable up to "debug3" level debug mode
    #endif
#endif


#if 1 //Always defined
    #define m6_log_fatal(format, args...) _m6_log_out_(M6_LOG_LVL_FATAL, __LINE__, __FILE__, format, ## args )
    #define m6_log_fatal_va(format, va_list) _m6_log_out_va_(M6_LOG_LVL_FATAL, __LINE__, __FILE__, format, va_list )
#else
    #define m6_log_fatal(format, args...)
    #define m6_log_fatal_va(format, va_list)
#endif


#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_ERROR
    #define m6_log_error(format, args...) _m6_log_out_(M6_LOG_LVL_ERROR, __LINE__, __FILE__, format, ## args )
    #define m6_log_error_va(format, va_list) _m6_log_out_va_(M6_LOG_LVL_ERROR, __LINE__, __FILE__, format, va_list )
#else
    #define m6_log_error(format, args...)
    #define m6_log_error_va(format, va_list)
#endif


#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_WARN
    #define m6_log_warn(format, args...) _m6_log_out_(M6_LOG_LVL_WARN, __LINE__, __FILE__, format, ## args )
    #define m6_log_warn_va(format, va_list) _m6_log_out_va_(M6_LOG_LVL_WARN, __LINE__, __FILE__, format, va_list )
#else
    #define m6_log_warn(format, args...)
    #define m6_log_warn_va(format, va_list)
#endif

#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_INFO
    #define m6_log_info(format, args...) _m6_log_out_(M6_LOG_LVL_INFO, __LINE__, __FILE__, format, ## args )
    #define m6_log_info_va(format, va_list) _m6_log_out_va_(M6_LOG_LVL_INFO, __LINE__, __FILE__, format, va_list )
#else
    #define m6_log_info(format, args...)
    #define m6_log_info_va(format, va_list)
#endif


#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_DEBUG1
    #define m6_log_debug1(format, args...) _m6_log_out_(M6_LOG_LVL_DEBUG1, __LINE__, __FILE__, format, ## args )
    #define m6_log_debug1_va(format, va_list) _m6_log_out_va_(M6_LOG_LVL_DEBUG1, __LINE__, __FILE__, format, va_list )
#else
    #define m6_log_debug1(format, args...)
    #define m6_log_debug1_va(format, va_list)
#endif

#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_DEBUG2
    #define m6_log_debug2(format, args...) _m6_log_out_(M6_LOG_LVL_DEBUG2, __LINE__, __FILE__, format, ## args )
    #define m6_log_debug2_va(format, va_list) _m6_log_out_va_(M6_LOG_LVL_DEBUG2, __LINE__, __FILE__, format, va_list )
#else
    #define m6_log_debug2(format, args...)
    #define m6_log_debug2_va(format, va_list)
#endif

#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_DEBUG3
    #define m6_log_debug3(format, args...) _m6_log_out_(M6_LOG_LVL_DEBUG3, __LINE__, __FILE__, format, ## args )
    #define m6_log_debug3_va(format, va_list) _m6_log_out_va_(M6_LOG_LVL_DEBUG3, __LINE__, __FILE__, format, va_list )
#else
    #define m6_log_debug3(format, args...)
    #define m6_log_debug3_va(format, va_list)
#endif

typedef struct {
    m6_colour_t color;  //Color for the log level
    m6_bool source;     //Include the source filename and line
    m6_bool timestamp;  //Include the timestamp
    char* text;         //Short text such as "Warning" for each instance of this level
} m6_log_level_setting_t;


typedef enum {
    m6_log_tostdout,
    m6_log_tostderr,
    m6_log_tofile,
    m6_log_tosyslog,
    //m6_log_http, -- Not yet implemented
} m6_log_output_e;

typedef struct {
    m6_word log_level;              //Current log level for contorling verbosity/output
    m6_bool use_color;              //Use colors when printing to a terminal
    m6_log_output_e output_mode;    //Where to output to
    char* filename;                 //If outputing to a file, the file name, otherwise ignored
    m6_bool use_utc;                //Use UTC time, otherwise use local time
    m6_bool incl_timezone;          //Include the UTC timezone offset
    m6_bool subsec_digits;          //How many sub-second digits to use
    m6_log_level_setting_t lvl_config[M6_LOG_LVL_COUNT]; //Setting for each log level
    int fd;                         //Private, the file descriptor of the output
} m6_log_settings_t;


//Only use this one in your code!
//You can customize most of these settings after calling this "function" to initialize
#define USE_M6_LOGGER(word_level,bool_use_color,enum_output_mode,str_filename) \
m6_log_settings_t m6_log_settings = { \
    .log_level      = word_level, \
    .use_color      = bool_use_color, \
    .output_mode    = enum_output_mode, \
    .filename       = str_filename, \
    .use_utc        = false, \
    .incl_timezone  = false, \
    .subsec_digits  = 0, \
    .lvl_config  = { \
        { .color = M6_TERM_COL_BRIGHT_RED,      .source = true,  .timestamp = true,  .text = "Fatal Error" }, /*FATAL*/\
        { .color = M6_TERM_COL_EMPH_RED,        .source = false, .timestamp = true,  .text = "Error"       }, /*ERROR*/\
        { .color = M6_TERM_COL_BRIGHT_YELLOW,   .source = false, .timestamp = true,  .text = "Warning"     }, /*WARNING*/\
        { .color = M6_TERM_COL_BRIGHT_GREEN,    .source = false, .timestamp = true,  .text = "Info"        }, /*INFO*/\
        { .color = M6_TERM_COL_NONE,            .source = false, .timestamp = false, .text = "Debug"       }, /*DEBUG 1*/\
        { .color = M6_TERM_COL_NONE,            .source = true,  .timestamp = false, .text = "Debug"       }, /*DEBUG 2*/\
        { .color = M6_TERM_COL_NONE,            .source = true,  .timestamp = false, .text = NULL          }  /*DEBUG 3*/\
    }, \
    .fd = -1,  /*This is private, please don't play with it*/\
}

#define USE_M6_LOGGER_DEFAULT USE_M6_LOGGER(M6_LOG_LVL_INFO,true,m6_log_tostderr,"")


#endif /* M6_LOG_H_ */
