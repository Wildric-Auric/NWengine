#pragma once
#include "Texture.h"
#include "Maths.h"
#include "Primitives.h"
#include "Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include <map>
#include <vector>
#include "NWstd.h"

//Virtual
class Drawable {
public:
	virtual void Draw(uint8_t slot = 0) {};
};

class Updatable {
public:
	virtual void Update() {};
};

class GameComponent {
public:
	static std::string GetType() { return "GameComponent"; }
};
//------------------------------------

class GameObject : public Drawable {
private:
	static int numberOfGameObjects;
public:
	std::map<std::string, void*> components;
	//int id;  //ReadOnly
	std::string name  = "new GameObject"; //Read only see Rename function;
	//identifier
	uint32 id = 0;
	bool isRendered = 1; //READONLY
	void AddToRender();
	void StopRendering();
	void Draw(int8 textureSlot = 0);
	void BasicDraw(int8 textureSlot = 0);
	void Rename(std::string newName);
    GameObject();

	template<typename T>
	T* GetComponent() {
		T* component = nullptr;
		
		if (components.find(T::GetType()) == components.end()) return component;

		component = (T*)components[T::GetType()];
		return component;
	};

	template<typename T>
	T* AddComponent() {
		T* ptr = new T(this);
		components.insert(std::pair<std::string, void*>( T::GetType(), ptr ));
		return ptr;
	};

	template<typename T>
	void DeleteComponent() {
		if (components.find(T::GetType()) == components.end()) return;
		delete components[T::GetType()];
		components.erase(T::GetType());
	}
};






//WARNING::Collider is deprecated see Collider2

class Collider {
private:
	Vector2<int> manualSize;
public:
	static std::string GetType() { return "Collider"; };
	Collider() {};
	Collider(GameObject* attachedObj, Vector2<int> offset = Vector2<int>(0, 0), Vector2<int>* newSize = nullptr);
	GameObject* attachedObj;  //TODO:: Make this GameObjectClone it's a mistake
	fVec2* position; //ReadOnly   //TODO:: Make it impossible to overwrite readonly variables
	iVec2* size; //ReadOnly
	iVec2 offset;
	iVec2 GetPosition();
	iVec2 GetSize();
	void Resize(Vector2<int> newSize);

};





class Scriptable {
public:
	GameObject* goc = nullptr;
	Scriptable(GameObject* goc = nullptr, void* nws = nullptr) {
		this->goc = goc;
	};
	std::string __nwname = "None"; //ReadOnly variables will have double underscore in the beginning
	virtual ~Scriptable() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual std::string GetName() { return __nwname; };
	void* nws = nullptr;
};



