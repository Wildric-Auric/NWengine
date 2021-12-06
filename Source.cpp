#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include<fstream>
#include<map>
#include<cstdio>
#include<string>
void sizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
};

//Consts
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

GLFWwindow* InitContext()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_WIDTH, "TDS", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to init glfw window";
		return nullptr;
	}
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, sizeCallBack);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to init GLEW";
		return 0;
	}
	return window;
}


std::pair<const char*, const char*> parseShader(const char* path)
{
	std::string frag, vert = "";    //TODO: Make it directly in the heap
	unsigned int current = 0;
	std::ifstream file(path);
	for (std::string line; std::getline(file, line);)
	{
		if (line.find("//fragment shader") != -1)
		{
			current = 1;
		}
		else if (line.find("//vertex shader") != -1) current = 2;
		if (current == 1) frag += line + '\n';
		else if (current == 2) vert += line + '\n';
	}

	return std::make_pair(_strdup(&vert[0]), _strdup(&frag[0]));
}


class Triangle {
private:
	unsigned int VBO, VAO,EBO;
	float vertices[9] = {
	-0.5f, -0.5f, 0.0f, // left  
	 0.5f, -0.5f, 0.0f, // right 
	 0.0f,  0.5f, 0.0f  // top   
	};
	unsigned int indices[3] = {
				0,1,2,
	};
public:
	Triangle() {
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
	void Draw() {

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}
};

class Quad {
private:
	unsigned int VBO, VAO, EBO;
	float vertices[20] = {  
    //VertexPos                //uv
	-0.5f, -0.5f, 0.0f,      0.0f,0.0f,
	 0.5f, -0.5f, 0.0f,      1.0f,0.0f,
	 -0.5f,  0.5f, 0.0f,     0.0f,1.0f,
	 0.5f ,0.5f, 0.0f,       1.0f,1.0f,
	 
	};
	unsigned int indices[6] = {
		0,2,1,
		1,2,3
	};
public:
	Quad() {
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

	}
	void Draw() {

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};
class Shader {
public:
	unsigned int shaderProgram;
	Shader(const char* path = "Shader1.shader") {
		std::pair<const char*, const char*> shaderSrc = parseShader(path);

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &(shaderSrc.first), NULL);
		glCompileShader(vertexShader);
		int successInfo;
		char log[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successInfo);
		if (!successInfo) {
			glGetShaderInfoLog(vertexShader, 512, NULL, log);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION FAILED\n" << log << std::endl;
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &(shaderSrc.second), NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successInfo);
		if (!successInfo) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, log);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION FAILED\n" << log << std::endl;
		}
	    shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram); //TODO: Add error message
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
};


int main()
{

	GLFWwindow* window = InitContext();
	if (window == nullptr) return -1;

	
	Triangle* tr = new Triangle;
	Shader* defaultShader = new Shader();
	Quad* quad = new Quad();
	*defaultShader = Shader();
	*tr = Triangle();
	*quad = Quad();
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//tr->Draw();
		quad->Draw();
		glUseProgram(defaultShader->shaderProgram);
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}