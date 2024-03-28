#pragma once
#include "vector"
#include "Globals.h"

class Primitives {
public:
	static void Init();
	static void Destroy();
};

class QuadInternal {
public: 
	uint32 VBO = 0;
	uint32 EBO = 0;
	uint32 VAO = 0;
	static QuadInternal quadInstance;
	QuadInternal();
	QuadInternal(int a) {};
	void Draw();
	void Delete();	
};

class Quad {
public:
	int width = 0, height = 0; //ReadOnly
	void UpdateSize(float width, float height);
	Quad(float width = 1.0f, float height = 1.0f);
	void Draw();
};

