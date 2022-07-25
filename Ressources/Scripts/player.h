#pragma once
#include<windows.h>
#include "GameObject.h"
#include "NWscripting.h"

#define DLL_FUNC extern "C" __declspec(dllexport)

class player : public Scriptable {
public:
	GameObject* goc;
	NW* nws;

	std::string name() { return "player"; };
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
	player(GameObject* goc, NW* nws);
	~player() { delete this; };

};


DLL_FUNC Scriptable* __cdecl GetScript(GameObject* goc, NW* nws);