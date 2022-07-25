#pragma once
//This script is interface between user's scripts compiled to dll and the game engine
#include <Map>
#include <string>
#include <deque>
#include "Transform.h"

class NW {
private:
	std::map<std::string, bool> keys {
		{"up" , 0},
		{"right", 0},
		{"left" , 0},
		{"down" , 0}
	};
	std::deque<std::string> consoleStack;

public:
	std::map<std::string, std::map<GameObject*, GameComponent>*> componentsLists;
	static NW* ptr;
	void ConsoleWrite(std::string log);
	bool GetPressedKey(std::string key);

	void Start();
	void Update();
};