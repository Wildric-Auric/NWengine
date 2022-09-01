#include "player.h"

player::player(GameObject* goc) {
	this->goc = goc;
}

void player::Update() {
	printf("Test");
}


Scriptable* player::GetScript(GameObject* goc) {
	return new player(goc);
};