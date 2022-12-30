#pragma once
#include<string>

class Serializer {
public:
	Serializer() {}
	//Will be used before preprocessing 
	void GenerateMeta(std::string src, std::string dest);
};