#pragma once
#include<windows.h>
#include "GameObject.h"
#include "NWscripting.h"

#define DLL_FUNC extern "C" __declspec(dllexport)


 class dllTest : public Scriptable {
public:
	bool boolean = 1;
	GameObject* goc;
	NW* nws;
	 void Update();
	dllTest(GameObject* goc, NW* nws);
	~dllTest() { delete this; };
};


DLL_FUNC Scriptable* __cdecl GetScript(GameObject* goc, NW* nws);