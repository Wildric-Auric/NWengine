#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"
#include "Image.h"



Asset* Image::LoadFromFile(const char* path, void* unused) {
	stbi_set_flip_vertically_on_load(1);
	pixelBuffer = stbi_load(path, &width, &height, &channels, 3 + alpha);
	if (!pixelBuffer) {
		std::cout << "ERROR::Incorrect image path or corrupted image: " << path << std::endl;
		return nullptr;
	}
	return this;
};

void Image::Clean() {
	stbi_image_free(pixelBuffer);
};









