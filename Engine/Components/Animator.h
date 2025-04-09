#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Sprite.h"
#include "ComponentTypes.h"

class Animator : public GameComponent {
private:
	double cycle = 0; //Keeps track of time to update frame
public:
    //NW_ST_GET_TYPE_IMPL(Animator);
	Animator() {};
	Animator(GameObject* obj);
	uint16 currentFrame = 0;
	bool isActive = 1;

	Animation animation;
	void Update();
};
