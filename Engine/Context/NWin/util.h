#pragma once

#include <iostream>
namespace NWin {
	//Whatever is wraper of, the object should implement the operator "<<"
	#define display(msg) std::cout << msg << "\n";
	#define TO_STR_0(nonStr)   #nonStr
	#define TO_STR(nonStr) TO_STR_0(nonStr)

	#define WIN_DISPLAY_ERROR(expr) { int win_get_last_error_var; if ((win_get_last_error_var = GetLastError()) != 0) \
	{display("WARNING: GetLasterror yields: " << win_get_last_error_var << " after using the expr: " #expr "\n");}}

	#define WIN_CHECK_COMPLETE(expr, errorValue, code) if (((expr)) == (errorValue) )  \
		{ display("\n----------------------------\n" \
				  "ERROR at file: "  __FILE__  \
				  "\n\nLine:" TO_STR(__LINE__)  \
				  "\n\nDuring evaluation of the expression: " #expr "\n\n"); WIN_DISPLAY_ERROR(expr); display("----------------------------\n"); code;} \


	#define WIN_CHECK2(expr, errorValue) WIN_CHECK_COMPLETE(expr, errorValue,)
	#define WIN_CHECK21(expr, code)		 WIN_CHECK_COMPLETE(expr,0,code)
	#define WIN_CHECK(expr)				 WIN_CHECK_COMPLETE(expr, 0,)

	#define DEFAULT_WINDOW_POSITION_SIZE ((int)0x80000000)
	typedef unsigned long Word;
	typedef unsigned char uchar;
	typedef unsigned int  uint;

	struct Vec2 {
		int x = 0;
		int y = 0;
	};

	struct Rect {
		Vec2 pos  = { DEFAULT_WINDOW_POSITION_SIZE, DEFAULT_WINDOW_POSITION_SIZE };
		Vec2 size = { DEFAULT_WINDOW_POSITION_SIZE, DEFAULT_WINDOW_POSITION_SIZE };
	};

	extern Word bitOr(int number, Word args...);
}