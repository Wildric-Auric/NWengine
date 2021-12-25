#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include "Maths.h"


class Quad {
private:
	unsigned int VBO = 0, VAO = 0, EBO = 0;
	int width = 0, height = 0;
	float vertices[20];
	unsigned int indices[6] = {
		0,2,1,
		1,2,3
	};
public: 
	Vector2<int> position;
	Vector2<float> scale;
	Quad(Vector2<int> position = Vector2<int>(0.0f,0.f),float width = 10.0f, float height = 10.0f);
	void Draw();
};

class Triangle {
private:
	unsigned int VBO = 0, VAO = 0, EBO = 0;
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


#endif 