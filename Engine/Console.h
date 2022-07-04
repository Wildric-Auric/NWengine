#pragma once
#include "Globals.h"
#include <map>
#include "string"
#include "vector"

#define CONSOLE_DEBUG_MESSAGE 0
#define CONSOLE_ERROR_MESSAGE 1
#define CONSOLE_WARNING_MESSAGE 2

class Console {
public:
	static std::vector<std::pair<std::string, uint16>> stack;
	static std::vector<uint8> flags;
	static void Write(const char* str, uint8 FLAG = CONSOLE_DEBUG_MESSAGE);
};