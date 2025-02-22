#pragma once
#include <stdint.h>

typedef uint32_t ui32;
typedef int32_t  i32;
typedef uint8_t  ui8;
typedef int8_t   i8;
typedef uint64_t ui64;
typedef int64_t  i64;
typedef int8_t   byte;
typedef void*    gen;
typedef void*    addr;

#define CAST(type, val) ((type)(val))
#define READ(type, val) *((type*)(val))



