#include <GL/glew.h>
#include "Primitives.h"
#include "Globals.h"

TriangleInternal::TriangleInternal(int) {}

TriangleInternal TriangleInternal::triInstance = TriangleInternal(0);
QuadInternal QuadInternal::quadInstance = QuadInternal(0);

QuadInternal::QuadInternal(int a) {}

//TODO::Change datatype of indices?
static void SetGLObjs(GLObjectData& d, uint32* indices, int sizeofInd, float* vertices, int sizeofVert) {
    NW_GL_CALL(glGenBuffers(1, &d.EBO));
	NW_GL_CALL(glGenVertexArrays(1, &d.VAO));
	NW_GL_CALL(glGenBuffers(1, &d.VBO));

	NW_GL_CALL(glBindVertexArray(d.VAO));
	NW_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, d.VBO));
	NW_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.EBO));

	NW_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeofVert, vertices, GL_STATIC_DRAW));

	NW_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofInd, indices, GL_STATIC_DRAW));

	NW_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	NW_GL_CALL(glEnableVertexAttribArray(0));

	NW_GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	NW_GL_CALL(glEnableVertexAttribArray(1));
}

static void DelGLObjs(GLObjectData& d) {
    NW_GL_CALL(glDeleteVertexArrays(1, &d.VAO));
	NW_GL_CALL(glDeleteBuffers(1,	   &d.VBO));
	NW_GL_CALL(glDeleteBuffers(1,	   &d.EBO));
}

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
    SetGLObjs(_gldat, indices, sizeof(indices), vertices, sizeof(vertices));	
};

TriangleInternal::TriangleInternal() {
    float vertices[] = {
		//VertexPos                //uv
		-0.5, -0.5, 0.0f,    0.0f,0.0f,
		 0.5, -0.5, 0.0f,    1.0f,0.0f,
		 0.25,  0.5, 0.0f,    1.0f,1.0f,
	};
	uint32 indices[] = {
		0,2,1,
	};
    SetGLObjs(_gldat, indices, sizeof(indices), vertices, sizeof(vertices));	
}

void TriangleInternal::Draw() {
    NW_GL_CALL(glBindVertexArray(_gldat.VAO));
	NW_GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
}

Triangle::Triangle(float w, float h) {
    UpdateSize(w, h);
}

void Triangle::Draw() {
    TriangleInternal::triInstance.Draw(); 
}


void QuadInternal::Draw() {
	NW_GL_CALL(glBindVertexArray(_gldat.VAO));
	NW_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void QuadInternal::Delete() {
    DelGLObjs(_gldat);
}

Quad::Quad(float width, float height) {
	this->width = width;
	this->height = height;
}

void Primitive::UpdateSize(float width, float height) {
	this->width  = width;
	this->height = height;
};

void Quad::Draw() {
    QuadInternal::quadInstance.Draw();
}

void Primitives::Init() {
	QuadInternal::quadInstance = QuadInternal();
    TriangleInternal::triInstance = TriangleInternal();
}

void Primitives::Destroy() {
	QuadInternal::quadInstance.Delete();
}

