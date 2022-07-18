#include "dllTest.h"

Scriptable* GetScript(GameObject* goc) {
	std::cout << "Hello world" << std::endl;
	return  new dllTest(goc);
};


void dllTest::Update() {
		 std::cout << "Scripting is here!!" << std::endl;
}

dllTest::dllTest(GameObject* goc) {
	this->goc = goc;
};


