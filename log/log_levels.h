/*
 * log_levels.h
 *
 *  Created on: Jul 2, 2013
 *      Author: mgrosvenor
 */


//Logging levels - The higher the number, the more this is on the critical path
#define CH_LOG_LVL_FATAL    0   //0: Fatal errors that cause the program to terminate
#define CH_LOG_LVL_ERROR    1   //1: Errors that a user can detect and recover from with substantial effort
#define CH_LOG_LVL_WARN     2   //2: Warnings that do not hinder operation, but may lead to unexpected results
#define CH_LOG_LVL_INFO     3   //3: Generally useful output information
#define CH_LOG_LVL_DEBUG1   4   //4: Shallow depth debug logs
#define CH_LOG_LVL_DEBUG2   5   //5: Medium depth debug logs
#define CH_LOG_LVL_DEBUG3   6   //6: Very deep debug logs
#define CH_LOG_LVL_COUNT    7


//Options for output
#define CH_LOG_OUT_STDOUT 0
#define CH_LOG_OUT_STDERR 1
#define CH_LOG_OUT_SYSLOG 2
#define CH_LOG_OUT_FILE   3
