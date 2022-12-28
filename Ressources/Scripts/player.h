#pragma once
#include<windows.h>
#include "GameObject.h"
#include "NWReflection.h"
#define DLL_FUNC extern "C" __declspec(dllexport) //Deprecated

class player : public Scriptable, public Reflected {
public:
	//Don't edit this section-----------------------
	GameObject* goc;
	std::string __nwname = "player";
	std::string GetName() { return __nwname; };
	static Scriptable* GetScript(GameObject* goc);
	//Users variable here:
	SERIALIZED_ATTR float isRunning = 0.0f;
	SERIALIZED_ATTR int isJumping = 0;
	SERIALIZED_ATTR			int isGrounded = 0;
	float jumpingStartPosition = 0.0f;
	float jumpTime = 0.8f;
	SERIALIZED_ATTR float timer = 0.0f;
	float jumpHeight = 50.0f;
	float yspd = 0.0f;
	void Start() {};
	void Update();
    player(GameObject* goc) {
		this->goc = goc;
		NW_SERIAL_DATA
	};
	~player() { delete this; };
};
