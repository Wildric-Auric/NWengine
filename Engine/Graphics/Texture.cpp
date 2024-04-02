#include "Texture.h"
#include "Image.h"
#include "GL/glew.h"


void Texture::_GPUGen(uint8* pixelBuffer, TexChannelInfo info) {
	glGenTextures(1, &_glID);
	Bind();
	//Upscaling parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (info == TexChannelInfo::NW_R)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _size.x, _size.y, 0, info, GL_UNSIGNED_BYTE, pixelBuffer);
	
	if (this->_hasMipMap) {
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		return;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Texture::GenMipMap() {
	Bind();
	_hasMipMap = 1;
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::SetMinFilter(TexMinFilter value) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, value);
}

void Texture::SetMaxFilter(TexMaxFilter value) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, value);
}

void Texture::SetEdgesBehaviour(TexEdge value) {
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, value);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, value);
}

void Texture::Bind(uint32 slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _glID);
}


Asset* Texture::GetFromCache(void* identifier) {
	auto iter = Texture::resList.find(*(TextureIdentifier*)(identifier));
	if (iter == Texture::resList.end()) return nullptr;
	return &iter->second;
}

Asset* Texture::LoadFromFile(const char* path, TextureIdentifierPtr identifier) {
	if (!path)
		return nullptr;
	Image im;
	im.alpha = ((TextureIdentifier*)identifier)->alpha;
	im.LoadFromFile(path,nullptr);	
	return LoadFromBuffer((void*)&im, identifier);
}

Asset* Texture::LoadFromBuffer(void* buffer, void* data) {
	if (buffer == nullptr)
		return nullptr;
	Texture* result;
	TexChannelInfo inf;
	Image& im = *(Image*)buffer;
	int num       = im.channels + im.alpha;
	inf           = num == 1 ? NW_R : ( num == 3 ? NW_RGB : NW_RGBA );
	TextureIdentifierPtr identifier = (TextureIdentifierPtr)data;

	result = &(Texture::resList.emplace(*(TextureIdentifier*)identifier, Texture())).first->second;
	result->_size = { im.width, im.height };
	result->_hasMipMap = this->_hasMipMap;
	result->_GPUGen(im.pixelBuffer, inf);
	return result;
}

void Texture::Delete() {
	glDeleteTextures(1, &this->_glID);
	this->_glID = 0;
}

NW_IMPL_RES_LIST(TextureIdentifier, Texture)