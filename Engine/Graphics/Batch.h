#pragma once
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

#include <unordered_map>
#include <vector>


//TODO::CHANGE MAP WITH UNORDERED MAP IN ALL PARTS OF NWENGINE
class Batch {
private:
	uint32 VAO;
	uint32 VBO;
	uint32 EBO;

	int currentTexSlot = 0;
	int offset         = 0;

public:
	static const uint32 strideSize;
	static const uint32 strideSizeByte;


	static uint32* indices;
	static int indicesSize;
	static uint32 batchMaxQuads;
	static uint16 maxBatchTextures;

	static std::unordered_map<uint32, std::vector<Batch*>> batchMap; //Batches are allocated dynamically and it's pointer is owned by this map which should be deleted with scene

	static void ComputeIndices();

	std::string shader    = "";
	uint32	    layer     = 0;
	bool		isDynamic = 1;


	std::unordered_map<GameObject*, int> objs;       //The value is the offset in stride unit
	std::vector<float>					 vertices;
	std::unordered_map<Texture*, int>    textures;   //value  is a texure slot
	Batch();
	 
	bool Render(GameObject* go, float* stride);
	void BindTextures();

	void Draw();
	void Delete();

};
