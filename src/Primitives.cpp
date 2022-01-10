#include <GL/glew.h>
#include <glfw3.h>
#include "Primitives.h"
#include "ShaderManager.h"
#include "Globals.h"
#include "RessourcesLoader.h"
Triangle::Triangle() {
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindVertexArray(VAO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
}
void Triangle::Draw() {
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}



Quad::Quad(Vector2<int> position,float width, float height) : position(0, 0), width(10), height(10),
vertices{ 0.0f }
{
	float vertices[20] = {
		//VertexPos                //uv
		-width / 2, -height / 2, 0.0f,    0.0f,0.0f,
		 width / 2, -height / 2, 0.0f,    1.0f,0.0f,
		-width / 2,  height / 2, 0.0f,    0.0f,1.0f,
		 width / 2 , height / 2, 0.0f,    1.0f,1.0f,
	};

	this->position = position;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
};



void Quad::Draw() {
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}



Line::Line(Vector2<int> start, Vector2<int> end, Vector3<float> color): start(0,0), end(0,0), color(0.f,0.f,0.f) {
	float vertices[6] = {
		start.x, start.y, 0.0f,
		end.x  ,   end.y, 0.0f,
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Line::Draw() {
	glUseProgram(shaders["shader_simple"].shaderProgram);
	shaders["shader_simple"].SetMat4x4("uMvp", &(projectionMatrix * viewMatrix)[0][0]);
	shaders["shader_simple"].SetUniform3f("uColor", color.x, color.y, color.z);
	shaders["shader_simple"].SetUniform1f("uAlpha", alpha); //TODO::Add Vector4 template
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
}