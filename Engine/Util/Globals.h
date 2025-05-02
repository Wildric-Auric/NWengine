#pragma once

#include "Maths.h"

extern int print(const char* format, ...);
// For now using just printf, should output to console later
#define NW_LOG_ERROR(str)	print("%s", str)
#define NW_LOG_ERRORI(str)	print("%d", str)
#define NW_LOG_WARNING(str) print("%s", str)
#define NW_LOG(str)			print("%s", str)

#ifdef NW_RELEASE
#define NW_GL_TEST()
#define NW_GL_ERROR_NO_CHECK(a)
#else
#define NW_GL_ERROR_NO_CHECK(a)                                                                                                  \
	{                                                                                                                            \
		NW_LOG_ERROR("\n---------------------\nOpenGL ERROR: ");                                                                 \
		NW_LOG_ERRORI(a);                                                                                                        \
		NW_LOG_ERROR("; at line: ");                                                                                             \
		NW_LOG_ERRORI(__LINE__);                                                                                                 \
		NW_LOG_ERROR("; at file: ");                                                                                             \
		NW_LOG_ERROR(__FILE__);                                                                                                  \
		NW_LOG_ERROR("\n---------------------\n");                                                                               \
	}

#define NW_GL_TEST()                                                                                                             \
	{                                                                                                                            \
		int a;                                                                                                                   \
		do {                                                                                                                     \
			a = glGetError();                                                                                                    \
			if(a != GL_NO_ERROR) {                                                                                               \
				NW_LOG_ERROR("\n---------------------\nOpenGL ERROR: ");                                                         \
				NW_LOG_ERRORI(a);                                                                                                \
				NW_LOG_ERROR("; at line: ");                                                                                     \
				NW_LOG_ERRORI(__LINE__);                                                                                         \
				NW_LOG_ERROR("; at file: ");                                                                                     \
				NW_LOG_ERROR(__FILE__);                                                                                          \
				NW_LOG_ERROR("\n---------------------\n");                                                                       \
			}                                                                                                                    \
		} while(a != GL_NO_ERROR);                                                                                               \
	}
#endif

#define NW_GL_CALL(code)                                                                                                         \
	code;                                                                                                                        \
	NW_GL_TEST()

#ifdef NW_RELEASE
#define NW_AL_TEST(c) c;
#else
#define NW_AL_TEST(c)                                                                                                            \
	{                                                                                                                            \
		if((c) != 0) {                                                                                                           \
			NW_LOG_ERROR("\n---------------------\nOpenAL ERROR: ");                                                             \
			NW_LOG_ERROR(#c);                                                                                                    \
			NW_LOG_ERROR("; at line: ");                                                                                         \
			NW_LOG_ERRORI(__LINE__);                                                                                             \
			NW_LOG_ERROR("; at file: ");                                                                                         \
			NW_LOG_ERROR(__FILE__);                                                                                              \
			NW_LOG_ERROR("\n---------------------\n");                                                                           \
		}                                                                                                                        \
	}
#endif

#define NW_AL_CALL(code) NW_AL_TEST(code)
// arch is always an integer type with size of pointer of the architecture
typedef long arch;

#ifdef _MSC_VER

typedef __int8	int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8	 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

typedef __int8	i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;

typedef unsigned __int8	 ui8;
typedef unsigned __int16 ui16;
typedef unsigned __int32 ui32;
typedef unsigned __int64 ui64;

#else
typedef __INT8_TYPE__  int8;
typedef __INT16_TYPE__ int16;
typedef __INT32_TYPE__ int32;
typedef __INT64_TYPE__ int64;

typedef __UINT8_TYPE__	uint8;
typedef __UINT16_TYPE__ uint16;
typedef __UINT32_TYPE__ uint32;
typedef __UINT64_TYPE__ uint64;

typedef __INT8_TYPE__  i8;
typedef __INT16_TYPE__ i16;
typedef __INT32_TYPE__ i32;
typedef __INT64_TYPE__ i64;

typedef __UINT8_TYPE__	ui8;
typedef __UINT16_TYPE__ ui16;
typedef __UINT32_TYPE__ ui32;
typedef __UINT64_TYPE__ ui64;
#endif

extern template class Vector2<int>;
extern template class Vector3<int>;
extern template class Vector4<int>;
extern template class Vector2<float>;
extern template class Vector3<float>;
extern template class Vector4<float>;
extern template class Vector2<double>;
extern template class Vector3<double>;
extern template class Vector4<double>;
extern template class Matrix2<int>;
extern template class Matrix3<int>;
extern template class Matrix2<float>;
extern template class Matrix3<float>;
extern template class Matrix4<int>;
extern template class Matrix4<float>;

typedef Vector2<int>	iVec2;
typedef Vector3<int>	iVec3;
typedef Vector4<int>	iVec4;
typedef Vector2<uint32> uiVec2;
typedef Vector3<uint32> uiVec3;
typedef Vector4<int32>	uiVec4;
typedef Vector2<float>	fVec2;
typedef Vector3<float>	fVec3;
typedef Vector4<float>	fVec4;
typedef Vector2<double> dVec2;
typedef Vector3<double> dVec3;
typedef Vector4<double> dVec4;

typedef Vector2<int>	v2i;
typedef Vector3<int>	v3i;
typedef Vector4<int>	v4i;
typedef Vector2<uint32> v2ui;
typedef Vector3<uint32> v3ui;
typedef Vector4<int32>	v4i;
typedef Vector2<float>	v2f;
typedef Vector3<float>	v3f;
typedef Vector4<float>	v4f;
typedef Vector2<double> v2d;
typedef Vector3<double> v3d;
typedef Vector4<double> v4d;

typedef Matrix2<int>   iMat2;
typedef Matrix3<int>   iMat3;
typedef Matrix2<float> fMat2;
typedef Matrix3<float> fMat3;
typedef Matrix4<int>   iMat4;
typedef Matrix4<float> fMat4;

#define NW_UI32_MAX 0xFFFFFFFF
#define NW_UI64_MAX 0xFFFFFFFFFFFFFFFF
#define NW_I64_MIN	0x8000000000000000
#define NW_I32_MIN	0x80000000
#define NW_I32_MAX	0x7FFFFFFF
#define NW_I64_MAX	0x7FFFFFFFFFFFFFFF
