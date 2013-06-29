/*
 * Copyright  (C) Matthew P. Grosvenor, 2012, All Rights Reserved
 *
 * Camio error handling
 *
 */

#include "camio_errors.h"


void _eprintf_exit(int err_type, int line_no, const char* file, const char *format, ...)
{
   va_list arg;

   //Figure out how long the error header string will be when we make it
   const char* err_txt = err_type == CAMIO_ERR ? "Error" : "Warning";
               err_txt = err_type == CAMIO_DBG ? "Debug" : "Warning";
   const char* err_str = "Camio %s [%s:%d]: ";
   int err_str_full_len = snprintf(NULL,0,err_str,err_txt, file, line_no);
   //Make space for the error header string and the format string
   char* full_format = malloc(err_str_full_len + strlen(format));
   if(!full_format){
       //Catch the double fault
       fprintf(stderr, "Camio Error [%s:%d]: Could not allocate memory for error text! Panic!\n",  __FILE__, __LINE__);
       exit(-1);
   }
   sprintf(full_format,err_str,err_txt, file, line_no);
   strcpy(full_format + err_str_full_len, format);

   va_start(arg, format);
   vfprintf (stderr, full_format,arg);
   va_end (arg);

   free(full_format);

   if(err_type == CAMIO_ERR)
       exit(-1);
}



void eprintf_exit_simple(const char *format, ...)
{
   va_list args;
   va_start(args, format);
   vfprintf (stderr, format,args);
   va_end (args);
   exit(-1);
}

void veprintf_exit_simple(const char *format, va_list args)
{
   vfprintf (stderr, format,args);
   exit(-1);
}


