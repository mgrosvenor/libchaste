#ifndef STDINT_H_
#define STDINT_H_
#ifdef _WIN32
#define int8_t char
#define uint8_t unsigned char
#define int16_t short int
#define uint16_t unsigned short int
#define int32_t int
#define uint32_t unsigned int
#define int64_t long
#define uint64_t unsigned long
#define __func__ "Unknown"
#else
#include <stdint.h>
#endif
#endif
