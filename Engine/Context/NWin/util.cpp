#include "util.h"
#include <stdarg.h>
namespace NWin {

Word bitOr(int number,Word args...) {
	Word ret;
	ret = 0;
	va_list ptr;
	va_start(ptr, number);
	for (int i = 0; i < number; ++i) {
		ret |= va_arg(ptr, Word);
	}
	va_end(ptr);
	return ret;
}

}