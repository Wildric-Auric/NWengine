#pragma once
#include "Globals.h"
#include "GuiObject.h"
#include "Serialized.h"

#include <map>

#define ADD_COMPONENT(str, type) if (type == #str ) return this->AddComponent<str>();

//Virtual
class Drawable {
public:
	virtual void Draw(uint8_t slot = 0) {};
};

class Updatable {
public:
	virtual void Update() {};
};

class GameComponent: public GuiObject, public Serialized { //I've tried to do multiple inheritance on subclass of GameComponent 
										//but casting from GameComponent* to GuiObject* messes up virtual functions, I will read more about it
public:
	static std::string GetType() { return "GameComponent"; }
	virtual void Update() {};
	virtual void Start() {};
	virtual ~GameComponent() {};
};
//------------------------------------

class GameObject : public Serialized {
private:
	static int numberOfGameObjects;
public:
	std::map<std::string, GameComponent*> components;
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
	~GameObject();
	void DeleteComponent(std::string typeName);

	int Serialize(std::fstream* data, int offset);
	int Deserialize(std::fstream* data, int offset);

	template<typename T>
	T* GetComponent() {
		T* component = nullptr;
		
		if (components.find(T::GetType()) == components.end()) return component;

		component = (T*)components[T::GetType()];
		return component;
	};


	GameComponent* AddComponent(std::string type);
	template<typename T>
	T* AddComponent() {
		T* ptr = new T(this);
		components.insert(std::pair<std::string, GameComponent*>( T::GetType(), ptr ));
		return ptr;
	};

	GameComponent* GetComponent(std::string type);

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



