#pragma once
#include "Globals.h"
#include "GuiObject.h"
#include "Serialized.h"

#include <map>

#define ADD_COMPONENT(str, type) if (type == #str ) return this->AddComponent<str>();

class GameComponent: public GuiObject, public Serialized { //I've tried to do multiple inheritance on subclass of GameComponent 
										//but casting from GameComponent* to GuiObject* messes up virtual functions, I will read more about it
public:
	static std::string GetType() { return "GameComponent"; }
	virtual void Update() {};
	virtual void Start() {};
	virtual ~GameComponent() {};
};

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
		std::map<std::string, GameComponent*>::iterator temp = components.find(T::GetType());
		if (temp != components.end()) return (T*)(*&temp)->second;
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