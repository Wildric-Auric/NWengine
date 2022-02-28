#pragma once
#include "Texture.h"
#include "Maths.h"
#include "Primitives.h"
#include "Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderManager.h"
#include "map"


class Drawable {
public:
	virtual void Draw(uint8_t slot = 0) {};
};

class Updatable {
public:
	virtual void Update() {};
};

class GameObject {
private:
	static int numberOfGameObjects;
public:
	//int id;  //ReadOnly
	Vector2<float> scale;
	Vector2<int> position;
	Vector2<int> size;  //ReadOnly
	Quad container; 
	Texture* texture;
	Shader* shader;
	std::string name;
	
	void Draw(int8 textureSlot = 0);
	void BasicDraw(int8 textureSlot = 0);

	GameObject() {};
	GameObject(Texture* texture, Vector2<int> position = Vector2<int>(0, 0),
		Vector2<float> scale = Vector2<float>(1.0f, 1.0f),
		std::string name = "NULL",
		Shader* shader = nullptr,
		bool usingImageSize = 1, Vector2<int> size = (10, 10));
};



class GameObjectClone : public Drawable {
public:
	const char* name;
	GameObject* originalGameObject; //ReadOnly
	Vector2<float> scale = Vector2<float>(1.0f,1.0f);
	Vector2<int> position = Vector2<int>(0,0);
	GameObjectClone(){};
	GameObjectClone(GameObject* gameObject, const char* name = "None");

	void Draw(int8 slot);
	void BasicDraw(int8 slot);

	template<typename T>
	T* GetComponent() {
		T* component = nullptr;
		for (auto it = T::componentList.begin(); it != T::componentList.end(); it++) {
			if (it->first == this) {
				component = &(it->second);
				break;
			}
		}
		return component;
	};

	template<typename T>
	T* AddComponent() {
		T::componentList[this] = T(this);
		return &(T::componentList[this]);
	};

	template<typename T>
	void DeleteComponent() {
		T::componentList.erase(this);
	};
};


class Collider {
private: 
	Vector2<int> manualSize;
public:
	Collider() {};
	Collider(GameObjectClone* attachedObj, Vector2<int> offset = Vector2<int>(0, 0), Vector2<int>* newSize = nullptr);
	GameObject* attachedObj;
	Vector2<int>* position; //ReadOnly   //TODO:: Make it impossible to overwrite readonly variables
	Vector2<int>* size; //ReadOnly
	Vector2<int> offset;
	Vector2<int> GetPosition();
	void Resize(Vector2<int> newSize);

	static std::map<GameObjectClone*, Collider> componentList;
};