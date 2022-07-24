#include "NWscripting.h"
#include "Inputs.h"
#include "Console.h"

NW* NW::ptr = nullptr;


bool NW::GetPressedKey(std::string key) {
	auto pair = keys.find(key);
	if (pair == keys.end()) {
		//TODO::Warning message
		std::cout << "Warning";
		return 0;
	}
	return pair->second;
}

void NW::ConsoleWrite(std::string log) {
	consoleStack.push_front(log);
}

void NW::Update() {
	//Setting inputs
	keys["up"] = Inputs::up;
	keys["right"] = Inputs::right;
	keys["down"] = Inputs::down;
	keys["left"] = Inputs::left;
	//Setting console 
	for (std::string log : consoleStack) {
		Console::Write(log.c_str());
		consoleStack.pop_front();
	}

}