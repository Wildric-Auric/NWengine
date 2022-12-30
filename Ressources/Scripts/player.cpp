#include "player.h"

void player::Update() {

}


Scriptable* player::GetScript(GameObject* goc) {
	return new player(goc);
};