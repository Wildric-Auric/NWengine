/**
 * @file Batch.h
 * @brief This file contains the declaration of the Batch class.
 */

#pragma once

#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

#include <unordered_map>
#include <vector>

#define UNIFORM_VIEWxPROJ_STR		"uVP"
#define UNIFORM_TEXTURE_ARRAY_STR	"uTex"


typedef int BatchChannelNum;

struct BatchDescriptor {
    BatchChannelNum* unitNum; 
    uint32 size;
    uint32 rawNum;
};

/**
 * @class Batch
 * @brief The Batch class represents a batch of game objects to be rendered together.
*/
class Batch {
private:
	uint32 VAO;
	uint32 VBO;
	uint32 EBO;
	bool _shouldDraw   = 1; //Used for static batch only
	int currentTexSlot = 0;
	int offset         = 0;
    
    static int* uniformTexArr;


public:
	const static uint32 defaultStrideSize = 9;

	std::string shader    = "";
	int		    layer     = 0;
	bool		isDynamic = 1;

	std::unordered_map<GameObject*, int> objs;       //The value is the offset in stride unit
	std::vector<float>					 vertices;
	std::unordered_map<Texture*, int>    textures;   //value  is a texure slot
    std::vector<BatchChannelNum> additionalData;
	uint32 strideSize = defaultStrideSize;
	uint32 strideSizeByte = defaultStrideSize * 4;

	/**
  * @brief Default constructor for the Batch class.
  */
	Batch(uint32 stride);
    Batch(const BatchDescriptor&);
	~Batch();

	/**
  * @brief Renders a game object using the batch.
  * @param go The game object to render.
  * @param stride The stride of the vertex data.
  * @return True if the rendering is successful, false otherwise.
  */
	bool Render(GameObject* go, float* stride);

	/**
  * @brief Binds the textures associated with the batch.
  */
	void BindTextures();

	/**
  * @brief Draws the batch.
  */
	void Draw();

	/**
  * @brief Deletes the batch.
  */
	void Delete();

	static uint32* indices;
	static int indicesSize;
	static uint32 batchMaxQuads;
	static uint16 maxBatchTextures;

	static std::unordered_map<int, std::vector<Batch*>> batchMap; //Batches are allocated dynamically and it's pointer is owned by this map which should be deleted with scene

    static void Init();

    static void Clear();
    
    static void Destroy();
	/**
  * @brief Computes the indices for the batch.
  */
	static void ComputeIndices();

	/**
  * @brief Default batch draw callback function.
  * @param data The data for the draw callback.
  * @return The result of the draw callback.
  */
	static int DefaultBatchDrawCallback(void* data);
};
