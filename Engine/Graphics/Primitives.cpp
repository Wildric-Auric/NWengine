#include <GL/glew.h>
#include "Primitives.h"
#include "Shader.h"
#include "Globals.h"
#include "Camera.h"

QuadInternal QuadInternal::quadInstance = QuadInternal(0);

QuadInternal::QuadInternal(int a) {}

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
	
	NW_GL_CALL(glGenBuffers(1, &EBO));
	NW_GL_CALL(glGenVertexArrays(1, &VAO));
	NW_GL_CALL(glGenBuffers(1, &VBO));

	NW_GL_CALL(glBindVertexArray(VAO));
	NW_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	NW_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));

	NW_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW));

	NW_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW));

	NW_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	NW_GL_CALL(glEnableVertexAttribArray(0));

	NW_GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	NW_GL_CALL(glEnableVertexAttribArray(1));
};

void QuadInternal::Draw() {
	NW_GL_CALL(glBindVertexArray(VAO));
	NW_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void QuadInternal::Delete() {
	NW_GL_CALL(glDeleteVertexArrays(1, &VAO));
	NW_GL_CALL(glDeleteBuffers(1,		&VBO));
	NW_GL_CALL(glDeleteBuffers(1,		&EBO));
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
	NW_GL_CALL(glBindVertexArray(QuadInternal::quadInstance.VAO));
	NW_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void Primitives::Init() {
	QuadInternal::quadInstance = QuadInternal();
}

void Primitives::Destroy() {
	QuadInternal::quadInstance.Delete();
}

