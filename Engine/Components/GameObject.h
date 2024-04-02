#pragma once
#include "Globals.h"
#include "Serialization.h"

#include <map>

#define ADD_COMPONENT(str, type) if (type == #str ) return this->AddComponent<str>();

typedef unsigned int (*DrawCallback)(void*);

class GameComponent: public Serialized { //I've tried to do multiple inheritance on subclass of GameComponent 
										//but casting from GameComponent* to GuiObject* messes up virtual functions, I will read more about it
public:
	static std::string GetType() { return "GameComponent"; }
	virtual void  Update() {};
	virtual void  Start() {};
	virtual void  SetGameObject(void* go) {};
	virtual void* GetGameObject() { return nullptr; };
	virtual ~GameComponent() {};
};

class GameObject : public Serialized {
private:
	static int numberOfGameObjects;
public:
	std::map<std::string, GameComponent*> components;
	//int id;  //ReadOnly
	std::string name	   = "new GameObject"; //Read only see Rename function;
	//identifier
	uint32 id			   = 0;
	//Draw callback, will be called at each Draw call
	DrawCallback _drawProc = nullptr;
	//Constructors
    GameObject();
	GameObject(const GameObject& other);
	~GameObject();

	//Functions
	uint32 Draw();  //Returns layer in which it has been drawn
	void SetDrawCallback(DrawCallback);
	void DeleteComponent(std::string typeName);
	void DeleteComponents();

	int Serialize(std::fstream* data, int offset);
	int Deserialize(std::fstream* data, int offset);

	template<typename T>
	T* GetComponent() {
		T* component = nullptr;
		if (this->components.find(T::GetType()) == this->components.end()) return component;

		component = (T*)this->components[T::GetType()];
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