/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Camio error handling
 *
 */


#ifndef CAMIO_ERRORS_H_
#define CAMIO_ERRORS_H_

#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>


#define CAMIO_ERR                    0x00
#define CAMIO_WARN                   0x01
#define CAMIO_DBG                    0x02


void _eprintf_exit(int err_type, int line_no, const char* file, const char *format, ...);
void eprintf_exit_simple(const char *format, ...);
#define eprintf_exit(format, args...) _eprintf_exit(CAMIO_ERR, __LINE__, __FILE__, format, ## args)
#define wprintf(format, args...) _eprintf_exit(CAMIO_WARN, __LINE__, __FILE__, format, ## args)
#ifndef NDEBUG
    #define dbprintf(format, args...) _eprintf_exit(CAMIO_DBG, __LINE__, __FILE__, format, ## args))
#else
    #define dbprintf(format, args...)
#endif

void veprintf_exit_simple(const char *format, va_list args);

#endif /* CAMIO_ERRORS_H_ */
