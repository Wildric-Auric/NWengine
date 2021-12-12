#pragma once
#include "Maths.h"
class Quad {
private:
	unsigned int VBO, VAO, EBO;
	int width, height;
	float vertices[20];
	unsigned int indices[6] = {
		0,2,1,
		1,2,3
	};
public: 
	Vector2 position;
	Vector2 scale;
	Quad(Vector2 position = Vector2(0.0f,0.0f),float width = 10.0, float height = 10.0);
	void Draw();
};

class Triangle {
private:
	unsigned int VBO, VAO, EBO;
	float vertices[9] = {
	-0.5f, -0.5f, 0.0f, // left  
	 0.5f, -0.5f, 0.0f, // right 
	 0.0f,  0.5f, 0.0f  // top   
	};
	unsigned int indices[3] = {
				0,1,2,
	};
public:
	Triangle();
	void Draw();
};
