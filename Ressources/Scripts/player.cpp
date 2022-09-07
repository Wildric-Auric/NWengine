#include "player.h"

player::player(GameObject* goc) {
	this->goc = goc;
}

void player::Update() {

}


Scriptable* player::GetScript(GameObject* goc) {
	return new player(goc);
};