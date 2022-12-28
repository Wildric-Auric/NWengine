#pragma once
#include <string>
#include <map>
#include "Globals.h"

#define SERIALIZED_ATTR
#define NW_SERIAL_DATA


class Reflected {
	//Map Key is the variable name, First element of the tuple is the size in bytes, the second is the offset, the third is the type, the fourth is value
	std::map<std::string, std::tuple<int, int, std::string, std::string>> reflectionData;
	std::tuple<int, int, std::string, std::string> getReflectionData(std::string varName) {
		std::map<std::string, std::tuple<int, int, std::string,std::string>>::iterator value = this->reflectionData.find(varName);
		if (value == this->reflectionData.end()) //TODO::Maybe a log?
			return std::make_tuple<int, int, std::string, std::string>(0,0,"", "");
		return value->second;
	}
};