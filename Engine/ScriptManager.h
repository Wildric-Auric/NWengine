#include<string>
#include "Scripts/player.h"
#include "Scripts/test.h"
//Should write a program doing all of this

inline Scriptable* CreateScript(std::string name, GameObject* goc) {

	if (name == "player") return new player(goc);
    if (name == "test")   return new test(goc);
	return nullptr;

}


