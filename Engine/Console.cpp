#include "Console.h"


std::map<std::string, uint16> Console::stack;
std::vector<uint8> Console::flags;

void Console::Write(const char* str, uint8 FLAG) {

	if (stack.find(std::string(str)) == stack.end()) {
		stack.insert(std::pair<std::string, uint16>(str, 1));
		flags.push_back(FLAG);
		return;
	}
	
	stack[str] += 1;
}