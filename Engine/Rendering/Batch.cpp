#include "Batch.h"
#include <GL/glew.h>
#include "Sprite.h"

//TODO::Implement static batch

/*Each vertex has 36 byte of data :
* -------------------
* Position     : X,Y,Z ----> 12 byte
* UV		   : X,Y   ----> 8  byte
* Other        : X,Y,Z ----> 12 byte
* Texture slot : X     ----> 4  byte
*/

const uint32 Batch::strideSizeByte   = 36;
const uint32 Batch::strideSize       = 9;
uint32 Batch::batchMaxQuads			 = 2048; 
uint16 Batch::maxBatchTextures		 = 0;
uint32* Batch::indices				 = nullptr;
int		Batch::indicesSize			 = 0;

std::unordered_map<uint32, std::vector<Batch*>> Batch::batchMap;

Batch::Batch() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, Batch::batchMaxQuads * strideSizeByte * 4, nullptr ,GL_DYNAMIC_DRAW); //4 is sizeof float
	//TODO::Multiply per sizeof(uint32)?? DONE BUT UNTESTED)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(uint32), Batch::indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Batch::strideSizeByte, (const void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Batch::strideSizeByte, (const void*)12);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Batch::strideSizeByte, (const void*)20);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, Batch::strideSizeByte, (const void*)32);
	glEnableVertexAttribArray(3);
}

void Batch::BindTextures() {
	for (std::unordered_map<Texture*, int>::iterator iter = textures.begin(); iter != textures.end();) {
		if (iter->first == nullptr) continue;
		if (iter->second < 0) {
			iter = textures.erase(iter);
			continue;
		}
		iter->first->Bind(iter->second);

		if (isDynamic)
			iter->second = -1; //Resetting the counter, if no obj updates it till next call, it will be deleted

		++iter;
	}
}


void Batch::ComputeIndices() {
	delete[] indices;
	indices = new uint32[Batch::batchMaxQuads * 6];
	for (int j = 0; j < Batch::batchMaxQuads; j++) {
		int i  = 4*j;
		int k  = 6*j;
		indices[k]	 = i;
		indices[k+1] = i+2;
		indices[k+2] = i+1;
		indices[k+3] = i+1;
		indices[k+4] = i+2;
		indices[k+5] = i+3;
	}
	Batch::indicesSize = Batch::batchMaxQuads * 6;
}

bool Batch::Render(GameObject* go, float* stride) {
	Sprite* sprite = go->GetComponent<Sprite>();
	if (shader == "") {
		shader = sprite->shader->name;
		this->layer = sprite->sortingLayer;
	}
	//Case of addition not possible
	auto texture = textures.find(sprite->texture);
	if ( (sprite->sortingLayer != this->layer)  || 
		 (this->offset >= this->batchMaxQuads * Batch::strideSize * 4) ||
		 (sprite->shader->name != this->shader) || 
		 (texture == textures.end() && textures.size() >= Batch::maxBatchTextures) )
	{
		return 0;
	}
	//Otherwise

	//Setting texture attribute
	if (texture == textures.end()) {
		textures.insert(std::make_pair(sprite->texture, currentTexSlot++));
		return 1;
	}
	if (texture->second < 0)
		texture->second = currentTexSlot++;

	int objSlot = currentTexSlot;
	if (texture->second > -1)
		objSlot = texture->second;

	*(stride + Batch::strideSize   - 1) =  (float)objSlot;
	*(stride + 2*Batch::strideSize - 1) =  (float)objSlot;
	*(stride + 3*Batch::strideSize - 1) =  (float)objSlot;
	*(stride + 4*Batch::strideSize - 1) =  (float)objSlot;
	//Copying strides into vertices vector
	if (vertices.size() < offset + Batch::strideSize * 4) {
		for (int i = 0; i < Batch::strideSize * 4; vertices.push_back(stride[i++]));
	}
	else
		for (int i = 0; i < Batch::strideSize * 4; vertices[offset + i] = stride[i++]);
	
	offset += Batch::strideSize * 4; //update offset

	return 1;
	
}

void Batch::Draw() {
	if (offset < 1)
		return;

	Shader::resList[this->shader].Use();

	BindTextures();

	static int arr[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13, 14, 15, 16, 17, 18,19,20, 21,22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	Shader::resList[this->shader].SetUniformArrayi("uTex", arr, 32);

	int size = offset / (4 * Batch::strideSize);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
   
	glBufferSubData(GL_ARRAY_BUFFER, 0, offset * sizeof(float), &vertices[0]);
	glDrawElements(GL_TRIANGLES, 6 * size, GL_UNSIGNED_INT, 0);
	
	if (isDynamic) {
		offset = 0;
		this->currentTexSlot = 0;
	}
	glBindVertexArray(0);
};

void Batch::Delete() {}
