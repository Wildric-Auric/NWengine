#include "Text.h"
#include "Sprite.h"
#include "Transform.h"
#include "ft2build.h"
#include FT_FREETYPE_H

NW_FT_Lib TextSystem::lib;

bool TextSystem::Init() {
	return !FT_Init_FreeType((FT_Library*)(&TextSystem::lib));
}

void TextSystem::Destroy() {
	FT_Done_FreeType((FT_Library)TextSystem::lib);
}

Font::Font(std::string path) {
	LoadFont(path);
}

//TODO::Add ressources base class, refactor ressources loader:
bool Font::LoadFont(std::string path) {
	if (FT_New_Face((FT_Library)TextSystem::lib, path.c_str(), 0, (FT_Face*)&this->face)) {
		NW_LOG_ERROR("Error::Loading font error");
		return 0;
	}
	FT_Face f = (FT_Face)this->face;
	FT_Set_Pixel_Sizes(f, 0, 64);
	for (uint8 i = 0; i < 128; ++i) {
		if (FT_Load_Char(f, i, FT_LOAD_RENDER)) {
			printf("Error loading character");
			continue;
		}
		 Texture tex	  = Texture(std::to_string(i), f->glyph->bitmap.width, f->glyph->bitmap.rows, f->glyph->bitmap.buffer, 2, false, 1, 1);
		 charactersMap.emplace(i, Glyph());
		 Glyph* character   =	&charactersMap.find(i)->second;
	
		 character->texture =   tex;

		 character->size.x	  = f->glyph->bitmap.width;
		 character->size.y	  = f->glyph->bitmap.rows;

		 character->bearing.x = f->glyph->bitmap_left;
		 character->bearing.y = f->glyph->bitmap_top;

		 character->advance   =	f->glyph->advance.x;
	}

	this->name = path;
	return 1;
}

void Font::Delete() {
	FT_Done_Face((FT_Face)face);
}

Glyph::Glyph() {

}
