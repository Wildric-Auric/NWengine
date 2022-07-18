#pragma once
#include "GameObject.h"
#include<windows.h>


#define DLL_FUNC extern "C" __declspec(dllexport)


 class dllTest : public Scriptable {
public:
	 void Update();
	dllTest(GameObject* goc);
	~dllTest() { delete this; };
};


DLL_FUNC Scriptable* __cdecl GetScript(GameObject* goc);