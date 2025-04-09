#pragma once

#ifdef _MSC_VER
typedef __int8  i8;
typedef __int32 i32;
typedef __int64 i64;
typedef unsigned __int8   ui8;
typedef unsigned __int32  ui32;
typedef unsigned __int64  ui64;
typedef __int8 ds_byte;
#else
typedef __INT8_TYPE__    i8;
typedef __INT32_TYPE__   i32;
typedef __INT64_TYPE__   i64;
typedef __UINT8_TYPE__   ui8;
typedef __UINT32_TYPE__  ui32;
typedef __UINT64_TYPE__  ui64;
typedef __INT8_TYPE__    byte;
typedef __INT8_TYPE__    ds_byte;
#endif
typedef void*    gen;
typedef void*    addr;

#define CAST(type, val) ((type)(val))
#define READ(type, val) *((type*)(val))



