#include "dllTest.h"

Scriptable* GetScript(GameObject* goc, NW* nws) {
	return  new dllTest(goc, nws);
};


void dllTest::Update() {
	if (nws->GetPressedKey("left")) {
		nws->ConsoleWrite("Toc-toc test");
	}
	boolean = 0;
}


dllTest::dllTest(GameObject* goc, NW* nws) {
	this->nws = nws;
	this->goc = goc;
};


