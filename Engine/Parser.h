#pragma once
#include<vector>
#include<string>
#include "Globals.h"

static enum Keys {
	GAME_OBJECT,
	COMPONENT,
	CAMERA
};

class Parser {
private:
	std::vector<uint16> keyStack;
public:
	std::vector<int16> tree; //Children number + grandchildren
	std::vector<uint16> keyLoc;
	std::vector<std::string> dataArray; 
	std::vector<std::string> rawData; //Contains literally lines of the file
	void Clear();
	Parser() {};
	void Parse(std::string path);
	~Parser() {
		Clear();
	}
};