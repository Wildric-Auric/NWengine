#include "Text.h"
#include <GL/glew.h>
#include <glfw3.h>

Font::Font(const char* font) {
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	if (FT_New_Face(ft, font, 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            Vector2<int>(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vector2<int>(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        chars.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(float) * 2));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
   
}



void Font::DisplayText(std::string text, Vector2<int> position, Shader* shader, Vector3<float> textColor, uint8_t slot) {
    glUseProgram(shader->shaderProgram);
    shader->SetUniform3f("uColor", textColor.x, textColor.y, textColor.z);
    glActiveTexture(slot);
    glBindVertexArray(VAO);
    for (char letter : text) {
        Character ftChar = chars[letter];
        float x = position.x + ftChar.bearing.x*5.0f;
        float y = position.y - ftChar.size.y*5.0f + ftChar.bearing.y*5.0f;
        float width = ftChar.size.x*5.0f;
        float height = ftChar.size.y*5.0f;
        float vertices[24] = {
            x, y + height, 0.0f, 0.0f,
            x, y, 0.0f,1.0f,
            x + width, y, 1.0f, 1.0f,
            x, y + height, 0.0f, 0.0f,
            x + width, y, 1.0f,1.0f,
            x + width, y + height, 1.0f, 0.0f
        };
        glBindTexture(GL_TEXTURE_2D, ftChar.id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ftChar.advance >> 6)* 5.0f;
    };

}