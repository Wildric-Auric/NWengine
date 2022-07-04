#include "Console.h"


std::vector<std::pair<std::string, uint16>> Console::stack;
std::vector<uint8> Console::flags;

void Console::Write(const char* str, uint8 FLAG) {
	bool condition = 1;
	uint16 index = 0;
	uint16 count = 0;
	for (auto it = stack.begin(); it != stack.end(); it++) {
		if (it->first == std::string(str)) {
			condition = 0;
			index = count;
			break;
		}
		count += 1;
	};


	if (condition) {
		stack.push_back(std::pair<std::string, uint16>(std::string(str), 1));
		flags.push_back(FLAG);
		return;
	}
	
	stack[index].second += 1;
}