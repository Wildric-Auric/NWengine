#pragma once
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


class Line {
private:
	unsigned int VBO = 0, VAO = 0;
	Vector2<int> start;
	Vector2<int> end;
	//float vertices[6];
public:
	Line(Vector2<int> start = Vector2<int>(), Vector2<int> end = Vector2<int>(), Vector3<float> color = Vector3<float>(1.0f, 1.0f, 1.0f));
	Vector3<float> color;
	float alpha = 1.0f;
	void Draw();
};

class Square {
//ONLY FOR DEBUGGING!
private: 

public:
	Vector2<int> position;
	Quad quad;
	Vector3<float> color;
	int size = size;
	float alpha = 1.0f;
	Square(Vector2<int> position, int size = 32, Vector3<float> color = Vector3<float>(0, 0, 0), float alpha = 1.0f);
	void Draw();
};

