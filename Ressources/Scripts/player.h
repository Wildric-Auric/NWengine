#pragma once
#include<windows.h>
#include "GameObject.h"

#define DLL_FUNC extern "C" __declspec(dllexport) //Deprecated

class player : public Scriptable {
public:
	//Don't edit this section-----------------------
	GameObject* goc;
	std::string __nwname = "player";
	std::string GetName() { return __nwname; };
	static Scriptable* GetScript(GameObject* goc);
	//Users variable here:
	float isRunning = 0.0f;
	int isJumping = 0;
	int isGrounded = 1;
	float jumpingStartPosition = 0.0f;
	float jumpTime = 0.8f;
	float timer = 0.0f;
	float jumpHeight = 50.0f;
	float yspd = 0.0f;
	void Start() {};
	void Update();
	player(GameObject* goc);
	~player() { delete this; };
};
