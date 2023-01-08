#include<vector>
#include<string>
#include<fstream>

//#define ADD_TO_SERIAL(data, index, element) if (data.size() <= index) data.push_back(element);\
//											else data[index] = element;

#define WRITE_ON_BIN(data, value, size, sizeBuffer) sizeBuffer = size;\
											 data->write((const char*)&sizeBuffer, sizeof(sizeBuffer));\
											 data->write((const char*)value, size);

#define READ_FROM_BIN(data, dest, sizeBuffer)		data->read((char*)&sizeBuffer, sizeof(int));\
													data->read((char*)dest, sizeBuffer);



class Serialized {
public:
	Serialized(){};
	int virtual Serialize(std::fstream* data, int offset) { return -1; }
	int virtual Deserialize(std::fstream* data, int offset) { return -1; }
};