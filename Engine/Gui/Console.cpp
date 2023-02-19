#include "Console.h"


std::vector<ConsoleText> Console::stack;

void Console::Write(const char* str, uint8 FLAG) {
	bool condition = 1;
	uint16 index = 0;
	uint16 count = 0;
	for (auto it = stack.begin(); it != stack.end(); it++) {
		if (it->str == std::string(str)  && it->flag == FLAG) {
			condition = 0;
			index = count;
			break;
		}
		count += 1;
	};

	
	if (condition) {
		ConsoleText ct = ConsoleText();
		ct.flag = FLAG;
		ct.str = str;
		stack.push_back(ct);
		return;
	}
	
	stack[index].number += 1;
}

void Console::Write(std::string str, uint8 FLAG) {
	Console::Write(str.c_str(), FLAG);
}

void Console::Write(int number, uint8 FLAG) {
	Console::Write(std::to_string(number), FLAG);
}

void Console::Write(float number, uint8 FLAG) {
	Console::Write(std::to_string(number), FLAG);
}

void Console::Write(double number, uint8 FLAG) {
	Console::Write(std::to_string(number), FLAG);
}

void Console::Clear() {
	stack.clear(); //I'm wrapping it with a function because extra processing may be done here in the future
}