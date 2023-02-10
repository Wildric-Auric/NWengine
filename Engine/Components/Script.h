#pragma once
#include "GameObject.h"

class Scriptable : GuiObject {
public:
	GameObject* goc = nullptr;
	Scriptable(GameObject* goc = nullptr, void* nws = nullptr) {
		this->goc = goc;
	};
	std::string __nwname = "None"; //ReadOnly variables will have double underscore in the beginning
	virtual ~Scriptable() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void Gui() {};

	virtual void ShaderCode(void* sprite);

	virtual std::string GetName() { return __nwname; };
	void* nws = nullptr;
};

class Script : public GameComponent {
public:
	static std::string GetType() { return "Script";};
	Script() {};
	Script(GameObject* attachedObj);
	GameObject* attachedObj = nullptr;
	Scriptable* script = nullptr;
	void Gui()	  override;
	void Update() override;
	int Serialize(std::fstream* data, int offset) override;
	int Deserialize(std::fstream* data, int offset) override;


	static std::map<GameObject*, Script> componentList;
};




