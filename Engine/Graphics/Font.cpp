#include "Font.h"
#include "Sprite.h"
#include "Transform.h"
#include "ft2build.h"
#include FT_FREETYPE_H

NW_FT_Lib Font::lib;

bool Font::Init() {
	return !FT_Init_FreeType((FT_Library*)(&Font::lib));
}

void Font::Destroy() {
	FT_Done_FreeType((FT_Library)Font::lib);
}

NW_IMPL_RES_LIST(FontIdentifier, Font)

Asset* Font::GetFromCache(void* identifier) {
	auto iter = Font::resList.find(*(FontIdentifier*)identifier);
	if (iter == Font::resList.end()) {
		return nullptr;
	}
	return (Asset*)&iter->second;
}

Asset* Font::LoadFromFile(const char* path, void* data = nullptr) {
	FT_Face tFace;
	if (FT_New_Face((FT_Library)Font::lib, path, 0, &tFace)) {
		NW_LOG_ERROR("Error::Loading font error");
		return nullptr;
	}
	std::string temp = std::string(path);
	return LoadFromBuffer(tFace, (void*)&temp);
}

Asset* Font::LoadFromBuffer(void* buffer, void* data) {
	Font& font = resList.emplace(*(FontIdentifier*)data, Font()).first->second;

	font.face = (FT_Face)buffer;
	FT_Face f = (FT_Face)font.face;

	FT_Set_Pixel_Sizes(f, 0, 64);
	for (uint8 i = 0; i < 128; ++i) {
		if (FT_Load_Char(f, i, FT_LOAD_RENDER)) {
			printf("Error loading character");
			continue;
		}

		font.charactersMap.emplace(i, Glyph());
		Glyph* character = &font.charactersMap.find(i)->second;


		character->texture._hasMipMap = 1;
		character->texture._size.x = f->glyph->bitmap.width;
		character->texture._size.y = f->glyph->bitmap.rows;
		character->texture._GPUGen(f->glyph->bitmap.buffer, TexChannelInfo::NW_R);

		character->size.x = f->glyph->bitmap.width;
		character->size.y = f->glyph->bitmap.rows;

		character->bearing.x = f->glyph->bitmap_left;
		character->bearing.y = f->glyph->bitmap_top;

		character->advance = f->glyph->advance.x;
	}
	return (Asset*)&font;
}

void Glyph::Delete() {
	this->texture.Clean();
}	

void Font::Clean() {
	--_usageCounter;
	if (_usageCounter > 0)
		return;
	for (auto iter = charactersMap.begin(); iter != charactersMap.end(); ++iter) {
		iter->second.Delete();
	}
	FT_Done_Face((FT_Face)face);
	EraseRes<Font>(GetIDWithAsset<Font*, FontIdentifier>(this));
}
