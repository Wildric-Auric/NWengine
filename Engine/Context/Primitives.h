#pragma once
#include "vector"
#include "Globals.h"

class Quad {
private:
	uint32 VBO = 0;
	uint32 EBO = 0;
	uint32 VAO = 0;
	std::vector<float> vertices;
	unsigned int indices[6] = {
		0,2,1,
		1,2,3
	};
public: 
	int width = 0, height = 0; //ReadOnly
	void UpdateSize(float width, float height);
	void Delete();
	Quad(float width = 1.0f, float height = 1.0f);
	void Draw();
	~Quad();
};

class Triangle {
private:
	uint32 VBO = 0;
	uint32 EBO = 0;
	uint32 VAO = 0;

	std::vector<float> vertices = {
	-0.5f, -0.5f, 0.0f, // left
	 0.5f, -0.5f, 0.0f, // right
	 0.0f,  0.5f, 0.0f  // top
	};
	uint32 indices[3] = {
				0,1,2,
	};
public:
	Triangle();
	void Draw();
};