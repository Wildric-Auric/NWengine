#include "Texture.h"
#include "Image.h"
#include "GL/glew.h"


void Texture::_GPUGen(uint8* pixelBuffer, TexChannelInfo info) {
	NW_GL_CALL(glGenTextures(1, &_glID));
	Bind();
	//Upscaling parameter
	NW_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	NW_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	NW_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	if (info == TexChannelInfo::NW_R)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	NW_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _size.x, _size.y, 0, info, GL_UNSIGNED_BYTE, pixelBuffer));
	
	if (this->_hasMipMap) {
		NW_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
		NW_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
		return;
	}
	NW_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
}

void Texture::GenMipMap() {
	Bind();
	_hasMipMap = 1;
	NW_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture::SetMinFilter(TexMinFilter value) {
	Bind();
	NW_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, value));
}

void Texture::SetMaxFilter(TexMaxFilter value) {
	Bind();
	NW_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, value));
}

void Texture::SetEdgesBehaviour(TexEdge value) {
	Bind();
	NW_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, value));
	NW_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, value));
}

void Texture::Bind(uint32 slot) {
	NW_GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
	NW_GL_CALL(glBindTexture(GL_TEXTURE_2D, _glID));
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

void Texture::Clean() {
	--_usageCounter;
	if (_usageCounter > 0) 
		return;
	NW_GL_CALL(glDeleteTextures(1, &this->_glID));
	this->_glID = 0;
	EraseRes<Texture,TextureIdentifier>(GetIDWithAsset<Texture*, TextureIdentifier>(this));
}

NW_IMPL_RES_LIST(TextureIdentifier, Texture)


void MSTexture::_GPUGen(TexChannelInfo channelInfo) {
	NW_GL_CALL(glGenTextures(1, &_glID));
	Bind();
	NW_GL_CALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samplesNum, channelInfo, _size.x, _size.y, GL_TRUE));
	Bind(1);
}

void MSTexture::Bind(bool unbind) {
	NW_GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, unbind ? 0 : _glID));
}

void MSTexture::Clean() {
	if (_glID == 0) return;
	NW_GL_CALL(glDeleteTextures(1, &this->_glID));
	this->_glID = 0;
}
