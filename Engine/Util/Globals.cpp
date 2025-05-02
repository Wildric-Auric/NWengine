#include "Globals.h"
#include "stdf.h"
#include <stdarg.h>
#include <stdio.h>
template class Vector2<int>;
template class Vector3<int>;
template class Vector4<int>;
template class Vector2<float>;
template class Vector3<float>;
template class Vector4<float>;
template class Vector2<double>;
template class Vector3<double>;
template class Vector4<double>;
template class Matrix2<int>;
template class Matrix3<int>;
template class Matrix2<float>;
template class Matrix3<float>;
template class Matrix4<int>;
template class Matrix4<float>;

int print(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int result = vprintf(format, args);
	va_end(args);
	return result;
}
