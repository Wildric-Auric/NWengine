#pragma once
#include "GameObject.h"

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
	//Users variable here:

	void Start() {};
	void Update();

};
