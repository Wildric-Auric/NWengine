#include "dllTest.h"

Scriptable* GetScript(GameObject* goc) {
	return  new dllTest(goc);
};


void dllTest::Update() {
	if (Inputs::n_0)
		if (boolean) std::cout << "Input Test" << std::endl;
	boolean = 0;

}


dllTest::dllTest(GameObject* goc) {
	this->goc = goc;
};


