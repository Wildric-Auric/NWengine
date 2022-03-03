#pragma once

#include "GameObject.h"
#include <string>

class test : public Scriptable {
public:
	std::string name() { return "test"; };

	float isRunning = 0.0f;
	int isJumping = 0;
	int isGrounded = 1;
	float jumpingStartPosition = 0.0f;
	float jumpTime = 0.8f;
	float timer = 0.0f;
	float jumpHeight = 50.0f;
	float yspd = 0.0f;
	virtual void Start() {};
	virtual void Update();
	test(GameObjectClone* goc) {
		this->goc = goc;
		goc->AddComponent<Collider>();
	};
};