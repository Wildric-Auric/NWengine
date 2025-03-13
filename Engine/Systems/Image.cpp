

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "Image.h"


Asset* Image::LoadFromFile(const char* path, void* unused) {
	stbi_set_flip_vertically_on_load(1);
	pixelBuffer = stbi_load(path, &width, &height, &channels, 3 + alpha);
	if (!pixelBuffer) {
		NW_LOG_ERROR("ERROR::Incorrect image path or corrupted image: ");
		NW_LOG_ERROR(path);
		NW_LOG_ERROR("\n");
		return nullptr;
	}
	return this;
};

int Image::SaveToFile(const char* path) {
  stbi_flip_vertically_on_write(1);
  return stbi_write_png(path, width, height, (channels+alpha), pixelBuffer, (channels+alpha) * width * sizeof(uint8));
}

void Image::Alloc() {
    pixelBuffer = (uint8*)malloc(sizeof(uint8) * width * height * (channels+alpha));
}

void Image::Clean() {
	stbi_image_free(pixelBuffer);
};









