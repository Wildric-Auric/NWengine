#include "Text.h"
#include "Sprite.h"
#include "Transform.h"

FT_Library TextSystem::lib;

bool TextSystem::Init() {
	return !FT_Init_FreeType(&TextSystem::lib);
}

void TextSystem::Destroy() {
	FT_Done_FreeType(TextSystem::lib);
}

Font::Font(std::string path) {
	LoadFont(path);
}

bool Font::LoadFont(std::string path) {
	if (FT_New_Face(TextSystem::lib, path.c_str(), 0, &face)) return 0;
	FT_Set_Pixel_Sizes(face, 0, 32);
	for (uint8 i = 0; i < 128; ++i) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			printf("Error loading character");
			continue;
		}
		 Texture tex	  = Texture(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer, 2, false, 1, 1);
		 tex.name         = i;
		 charactersMap.emplace(i, Glyph());
		 Glyph* character   =	&charactersMap.find(i)->second;
	
		 character->texture =   tex;

		 character->size.x	  = face->glyph->bitmap.width;
		 character->size.y	  = face->glyph->bitmap.rows;

		 character->bearing.x = face->glyph->bitmap_left;
		 character->bearing.y = face->glyph->bitmap_top;

		 character->advance   =	face->glyph->advance.x;

	}
	return 1;
}

void Font::Delete() {
	FT_Done_Face(face);
}

Glyph::Glyph() {

}
