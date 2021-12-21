#ifndef TEXT_H
#define TEXT_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <vector>
#include <string>

class Text
{
public:
	//FT_Library	ft;
	FT_Library ft;
	FT_Face face;
	
	int		initfreetype(const char *font);
	void	textRenderer(Shader &s, float x, float y, glm::vec2 scale, glm::vec3 color);
private:
	
};


#endif // !TEXT_H
