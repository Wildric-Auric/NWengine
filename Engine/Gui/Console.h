#pragma once
#include "Globals.h"
#include <map>
#include <string>
#include <vector>

#define CONSOLE_DEBUG_MESSAGE 0
#define CONSOLE_ERROR_MESSAGE 1
#define CONSOLE_WARNING_MESSAGE 2

struct ConsoleText {
	std::string str;
	uint16 number = 0;
	uint8 flag;
};


class Console {
public:
	static std::vector<ConsoleText> stack;
	static void Write(const char* str, uint8 FLAG = CONSOLE_DEBUG_MESSAGE);
};