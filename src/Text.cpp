//#include "Text.h"
//
//int		Text::initfreetype(const char *font)
//{
//	if (FT_Init_FreeType(&ft))
//	{
//		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//		return -1;
//	}
//	if (FT_New_Face(ft, font, 0, &face))
//	{
//		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//		return -1;
//	}
//
//	FT_Set_Pixel_Sizes(face, 0, 48);
//
//	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
//	{
//		std::cout << "Error: Failed to load Glyph" << std::endl;
//		return -1;
//	}
//
//	return 0;
//}
//
//void	Text::InitTextRenderer()
//{
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
//
//	for (size_t i = 0; i < 128; i++)
//	{
//		unsigned int texture;
//		glGenTextures(1, &texture);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
//			face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
//			face->glyph->bitmap.buffer);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		character c =
//		{
//			texture,
//			glm::ivec2(face->glyph->bitmap.width),
//			glm::ivec2(face->glyph->bitmap.rows),
//			face->glyph->advance.x
//		};
//		characters.insert(std::pair<char, character>(i, c));
//	}
//	FT_Done_Face(face);
//	FT_Done_FreeType(ft);
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
//
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FLOAT, 4 * sizeof(float), NULL);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//}
//
//void	Text::RenderText(Shader& s, std::string testslist, float x, float y, float scale, glm::vec3 color)
//{	
//	glUseProgram(s.shaderProgram);
//	s.SetUniform3f("color", color.x, color.y, color.z);
//	 Activate correspanding render state
//
//	 Iterate over 128 ascii characters
//	std::string::const_iterator c;
//	for (c = testslist.begin() ; c != testslist.end(); c++)
//	{
//		character	ch		= characters[*c];
//		float		xpos	= x + ch.bearing.x * scale;
//		float		ypos	= y - (ch.size.y - ch.bearing.y) * scale;
//
//		float		w		= ch.bearing.x * scale;
//		float		h		= ch.bearing.y * scale;
//
//		float	vertecies[6][4] =
//		{
//			{xpos	,	ypos + h,	0.0f, 0.0f},
//			{xpos	,	ypos,		0.0f, 1.0f},
//			{xpos + w,	ypos,		1.0f, 1.0f},
//			
//			{xpos	,	ypos + h,	0.0f, 0.0f},
//			{xpos + w,	ypos,		1.0f, 1.0f},
//			{xpos + w,	ypos,		1.0f, 0.0f},
//		};
//		glBindTexture(GL_TEXTURE_2D, ch.textureID);
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertecies), vertecies);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		x += (ch.advance >> 6) * scale;
//	}
//	glBindVertexArray(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//
