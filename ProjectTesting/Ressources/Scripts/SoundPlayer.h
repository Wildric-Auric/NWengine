#pragma once
#include "Script.h"

class SoundPlayer : public Scriptable {
public:
	//Don't edit this section-----------------------
	GameObject* goc;
	std::string __nwname = "SoundPlayer";
	std::string GetName() { return __nwname; };
	static Scriptable* GetScript(GameObject* goc);
	SoundPlayer(GameObject* goc) {
		this->goc = goc;
	};
	~SoundPlayer() { delete this; };
	//Users variable here:

	void Start() {};
	void Update();

};
