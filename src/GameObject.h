#pragma once
#include "Texture.h"
#include "Maths.h"
#include "ShaderManager.h"
#include "Primitives.h"
#include "Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class GameObject {
private:
	Quad container;
	
public:
	Vector2<float> scale;
	Vector2<int> position;
	Vector2<int> size;  //ReadOnly

	Texture* image;
	Shader* shader;

	void Draw(uint8_t textureSlot = 0);
	GameObject(Texture* image, Vector2<int> position = Vector2<int>(0 , 0), 
				Vector2<float> scale = Vector2<float> (1.0f, 1.0f), Shader* shader = shader_default, bool usingImageSize = 1, Vector2<int> size = (10, 10));
};


class Collider {
private: 
	Vector2<int> manualSize;
public:
	Collider(GameObject* attachedObj, Vector2<int> offset = Vector2<int>(0, 0),
				Vector2<int>* newSize = nullptr);
	GameObject* attachedObj;
	Vector2<int>* position; //ReadOnly   //TODO:: Make it impossible to overwrite readonly variables
	Vector2<int>* size; //ReadOnly
	Vector2<int> offset;
	Vector2<int> GetPosition();
	void Resize(Vector2<int> newSize);
};