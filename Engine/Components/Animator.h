#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Sprite.h"

class Animator : public GameComponent {
private:
	double cycle = 0; //Keeps track of time to update frame
public:
	static std::string GetType() { return "Animator"; };
	Animator() {};
	Animator(GameObject* attachedObj);
	GameObject* attachedObj = nullptr;
	uint16 currentFrame = 0;
	bool isActive = 1;

	Animation animation;
	void Update();
};