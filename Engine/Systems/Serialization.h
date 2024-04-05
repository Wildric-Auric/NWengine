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



/**
 * @class Serialized
 * @brief Base class for serialization and deserialization.
 */
class Serialized {
public:
	/**
  * @brief Default constructor for Serialized class.
  */
	Serialized() {};

	/**
  * @brief Serialize the object and write it to the file.
  * @param data The file stream to write the serialized data to.
  * @param offset The offset in the file to start writing the serialized data.
  * @return The number of bytes written to the file, or -1 if an error occurred.
  */
	int virtual Serialize(std::fstream* data, int offset) { return -1; }

	/**
  * @brief Deserialize the object from the file.
  * @param data The file stream to read the serialized data from.
  * @param offset The offset in the file to start reading the serialized data.
  * @return The number of bytes read from the file, or -1 if an error occurred.
  */
	int virtual Deserialize(std::fstream* data, int offset) { return -1; }
};