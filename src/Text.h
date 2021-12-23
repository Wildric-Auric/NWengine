#ifndef TEXT_H
#define TEXT_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <set>
#include "ShaderManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct character
{
	unsigned int	textureID;
	glm::ivec2		size;
	glm::ivec2		bearing;
	unsigned int	advance;
};

class Text
{
	private:

	public:

		unsigned int	VAO = 0, VBO = 0;

		FT_Library ft;
		FT_Face face;
		std::map<char, character> characters;

		int		initfreetype(const char *font);
		void	InitTextRenderer();
		void	RenderText(Shader &shader, std::string testslist, float x, float y, float scale, glm::vec3 color);
		std::string	UpdateTexts();
};

#endif 
