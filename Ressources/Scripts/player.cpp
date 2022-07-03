#include "player.h"
#include "NWengine.h"
#include "Utilities.h"


void player::Update() {
	isGrounded = 0;
	int vCol = 0;
	int direction = (Inputs::right - Inputs::left);
	goc->GetComponent<Transform>()->position.x += isRunning * 200 * Globals::deltaTime;
	goc->GetComponent<Transform>()->scale.x = sign(isRunning);

	float a = 0;
	if (direction == vCol) direction = 0;
	isRunning = Clamp((float)direction + a, -1.0f, 1.0f);
}