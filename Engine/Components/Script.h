#pragma once
#include "GameObject.h"

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

