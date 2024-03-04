#include "Texture.h"
#include "RessourcesLoader.h"
#include "GL/glew.h"
#include "glfw3.h"


std::map<std::string, Texture> Texture::resList;


Texture::Texture(const uint8* texRes, const iVec2& size, const TextureData& texData) {

	this->alpha =  texData.alpha;
	this->repeat = texData.repeat;
	this->name   = texData.name;
	this->size   = size;

	this->_texData = texData;

	int32 filter = GL_LINEAR;
	if (!this->_texData.linear)
		filter = GL_NEAREST;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	//Downscaling parameter
	if (this->_texData.genMipMap)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	//Upscaling parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	if (this->_texData.repeat) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	}
	if (this->_texData.alpha == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->size.x, this->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texRes);
	else if (this->_texData.alpha == 0)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->size.x, this->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, texRes);
	else if (this->_texData.alpha == 2) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->size.x, this->size.y, 0, GL_RED, GL_UNSIGNED_BYTE, texRes);
	}

	if (this->_texData.genMipMap)
		glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(std::string name, int width, int height, uint8* texRes, uint8 alpha, uint8 repeat, uint8 genMipMap, uint8 linear) :
Texture(texRes, iVec2(width, height), TextureData{ name, alpha, repeat, genMipMap, linear }) {}

void Texture::Bind(unsigned int slot = 0) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}
void Texture::UpdateTexture(int width, int height, uint8* texRes, unsigned int slot, bool alpha) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (alpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texRes);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texRes);

	glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture::UpdateTextureData(const TextureDataUpdate& texData) {
	Bind();
	if (texData.genMipMap != _texData.genMipMap) {
		_texData.genMipMap = texData.genMipMap;
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	if (texData.linear != _texData.linear) {
		_texData.linear = texData.linear;
		int32 filter = GL_LINEAR;
		if (!this->_texData.linear)
			filter = GL_NEAREST;
		if (this->_texData.genMipMap)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	}

	if (texData.repeat != _texData.repeat) {
		_texData.repeat = texData.repeat;
		if (this->_texData.repeat) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		}
	}
}

void Texture::Delete() {
	glDeleteTextures(1, &this->texture);
}