
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"
#include <string>
#include <fstream>
#include <vector>
#include "RessourcesLoader.h"
#include "FrameBuffer.h"
#include "Scene.h"
#include "Sprite.h"

std::map<std::string, Image> Image::resList;


Image::Image(std::string path, bool alpha) {
	stbi_set_flip_vertically_on_load(1);
	alpha                             =    alpha;
	tex                               =    stbi_load(path.c_str(),&width,&height,&channels,3+alpha);
	if (!tex) std::cout << "ERROR::Incorrect image path or corrupted image: " << path << std::endl;
};

void Image::Clear() {
	FreeBuffer(tex);
};


void FreeBuffer(uint8* buffer) {
	stbi_image_free(buffer);
};


Shader* RessourcesLoader::ReloadShader(std::string path) {
	Shader::resList[path] = Shader(path); //Name set in shader constructor, not like texture where no path is passed
	return &Shader::resList[path];
}

Shader* RessourcesLoader::LoadShader(std::string path) {
	if (Shader::resList.find(path) != Shader::resList.end()) {
		//std::cout << "WARNING::Trying to load already shader texture: " << path << std::endl;   //LOG 
		return &Shader::resList[path];
	}
	RessourcesLoader::ReloadShader(path);
	return &Shader::resList[path];
};




Texture* RessourcesLoader::LoadTexture(std::string path, bool alpha, bool repeat) {
	Image temp = Image(path, alpha);
	if (Texture::resList.find(path) != Texture::resList.end()) {
		//std::cout << "WARNING::Trying to load already loaded texture: " << path << std::endl;   //LOG 
		return &Texture::resList[path];
	}
	Texture::resList[path] = Texture(temp.width, temp.height, temp.tex, alpha, repeat);
	Texture::resList[path].name = path;
	Texture::resList[path].alpha = alpha;
	Texture::resList[path].repeat = repeat;
	temp.Clear();
	return &Texture::resList[path];
};

void RessourcesLoader::LoadDefaultRessources() {
	LoadShader(SHADER_DEFAULT);
	LoadTexture(TEXTURE_DEFAULT, 1, 1);
};

void RessourcesLoader::ClearUnusedTextures() {
	for (auto it = Scene::currentScene->sceneObjs.begin(); it != Scene::currentScene->sceneObjs.end(); it++) {
		Sprite* sprite = it->GetComponent<Sprite>();
		if (sprite == nullptr) continue;
		auto mapIt = Texture::resList.find(sprite->texture->name);
		if (mapIt == Texture::resList.end()) continue;
		Texture::resList.erase(mapIt);
	}
}


void RessourcesLoader::ClearAllTextures() {
	Texture::resList.clear();
}







