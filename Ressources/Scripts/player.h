#pragma once
#include "Script.h"

class player : public Scriptable {
public:
	//Don't edit this section-----------------------
	GameObject* goc;
	std::string __nwname = "player";
	std::string GetName() { return __nwname; };
	static Scriptable* GetScript(GameObject* goc);
	player(GameObject* goc) {
		this->goc = goc;
	};
	~player() { delete this; };
	void Start() {};
	void Update();
	void Gui() override;
	void ShaderCode(void* sprite) override;
	//Users variable here:

private:
	float speed		 = 100.0f;
	float uniformRed = 1.0f;
};
