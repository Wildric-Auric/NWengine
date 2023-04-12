#pragma once

#include<vector>
#include<string>
#include<fstream>

//#define ADD_TO_SERIAL(data, index, element) if (data.size() <= index) data.push_back(element);\
//											else data[index] = element;

#define WRITE_ON_BIN(data, valuePtr, size, sizeBuffer) sizeBuffer = size;\
											 data->write((const char*)&sizeBuffer, sizeof(sizeBuffer));\
											 data->write((const char*)valuePtr, size);

#define READ_FROM_BIN(data, destPtr, sizeBuffer)		data->read((char*)&sizeBuffer, sizeof(int));\
														data->read((char*)destPtr, sizeBuffer);



class Serialized {
public:
	Serialized(){};
	int virtual Serialize(std::fstream* data, int offset) { return -1; }
	int virtual Deserialize(std::fstream* data, int offset) { return -1; }
};