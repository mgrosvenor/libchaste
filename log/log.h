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

#ifndef M6_LOG_BUILD_LVL
    #if NDEBUG
        #define M6_LOG_BUILD_LVL   M6_LOG_LVL_INFO    //Output binary to be capable up to "info" level release mode
    #else
        #define M6_LOG_BUILD_LVL   M6_LOG_LVL_DEBUG3  //Output binary to be capable up to "debug3" level debug mode
    #endif
#endif


#if 1 //Always defined
    #define m6_log_fatal(format, args...) _m6_log_out_(M6_LOG_LVL_FATAL, __LINE__, __FILE__, format, ## args )
#else
    #define m6_log_fatal(format, args...)
#endif


#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_ERROR
    #define m6_log_error(format, args...) _m6_log_out_(M6_LOG_LVL_ERROR, __LINE__, __FILE__, format, ## args )
#else
    #define m6_log_error(format, args...)
#endif


#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_WARN
    #define m6_log_warn(format, args...) _m6_log_out_(M6_LOG_LVL_WARN, __LINE__, __FILE__, format, ## args )
#else
    #define m6_log_warn(format, args...)
#endif

#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_INFO
    #define m6_log_info(format, args...) _m6_log_out_(M6_LOG_LVL_INFO, __LINE__, __FILE__, format, ## args )
#else
    #define m6_log_info(format, args...)
#endif


#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_DEBUG1
    #define m6_log_debug1(format, args...) _m6_log_out_(M6_LOG_LVL_DEBUG1, __LINE__, __FILE__, format, ## args )
#else
    #define m6_log_debug1(format, args...)
#endif

#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_DEBUG2
    #define m6_log_debug2(format, args...) _m6_log_out_(M6_LOG_LVL_DEBUG2, __LINE__, __FILE__, format, ## args )
#else
    #define m6_log_debug2(format, args...)
#endif

#if M6_LOG_BUILD_LVL >= M6_LOG_LVL_DEBUG3
    #define m6_log_debug3(format, args...) _m6_log_out_(M6_LOG_LVL_DEBUG3, __LINE__, __FILE__, format, ## args )
#else
    #define m6_log_debug3(format, args...)
#endif

typedef struct {
    m6_colour_t color;
    m6_bool source;
    m6_bool timestamp;
    char* text;
} m6_log_level_setting_t;


typedef enum {
    m6_log_tostdout,
    m6_log_tostderr,
    m6_log_tofile,
    m6_log_tosyslog,
    //m6_log_http, -- Not yet implemented
} m6_log_output_e;

typedef struct {
    m6_bool use_color;
    m6_log_output_e output_mode;
    char* filename;
    int fd;
    m6_bool use_utc;
    m6_bool incl_timezone;
    m6_bool subsec_digits;
    m6_log_level_setting_t lvl_config[M6_LOG_LVL_COUNT];
} m6_log_settings_t;


//Only use this one in your code!
//You can customize most of these settings after calling this "function" to initialize
#define USE_M6_LOGGER(bool_use_color,enum_output_mode,str_filename) \
m6_log_settings_t m6_log_settings = { \
    .fd = -1, \
    .use_color   = bool_use_color, \
    .output_mode = enum_output_mode, \
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
}}


m6_word _m6_log_out_(m6_word this_log_lvll, m6_word line_num, const char* filename, const char* format, ... );
m6_word _m6_log_out_va_(m6_word this_log_lvll, m6_word line_num, const char* filename, const char* format, va_list args);



#endif /* M6_LOG_H_ */
