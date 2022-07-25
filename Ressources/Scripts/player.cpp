#include "player.h"

player::player(GameObject* goc, NW* nws) {
	this->goc = goc;
	this->nws = nws;
}

void player::Update() {
	float s = nws->GetPressedKey("right") - nws->GetPressedKey("left");
	goc->GetComponent<Transform>()->position.x += s;
}


Scriptable* GetScript(GameObject* goc, NW* nws) {
	return new player(goc, nws);
};