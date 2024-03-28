#include <GL/glew.h>
#include "Primitives.h"
#include "Shader.h"
#include "Globals.h"
#include "RessourcesLoader.h"
#include "Camera.h"

QuadInternal QuadInternal::quadInstance = QuadInternal(0);

QuadInternal::QuadInternal()
{
    float vertices[] = {
		//VertexPos                //uv
		-0.5, -0.5, 0.0f,    0.0f,0.0f,
		 0.5, -0.5, 0.0f,    1.0f,0.0f,
		-0.5,  0.5, 0.0f,    0.0f,1.0f,
		 0.5,  0.5, 0.0f,    1.0f,1.0f,
	};

	uint32 indices[] = {
		0,2,1,
		1,2,3
	};
	
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
};

void QuadInternal::Draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void QuadInternal::Delete() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1,		&VBO);
	glDeleteBuffers(1,		&EBO);
}


Quad::Quad(float width, float height) {
	this->width = width;
	this->height = height;
}

void Quad::UpdateSize(float width, float height) {
	this->width  = width;
	this->height = height;
};

void Quad::Draw() {
	glBindVertexArray(QuadInternal::quadInstance.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Primitives::Init() {
	QuadInternal::quadInstance = QuadInternal();
}

void Primitives::Destroy() {
	QuadInternal::quadInstance.Delete();
}

