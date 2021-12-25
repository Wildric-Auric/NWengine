#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"
#include <string>
#include <iostream>

#include "RessourcesLoader.h"

const char* path = "Ressources/Images/img2.png";
Image IMAGE_APPLE;

Image::Image(const char *path, bool alpha) {
	stbi_set_flip_vertically_on_load(1);
	alpha = alpha;
	tex = stbi_load(path, &width, &height, &channels, 3 + alpha);
}

void LoadImages() {
	IMAGE_APPLE = Image(path, true);
}
void FreeBuffer(unsigned char* buffer) {
	stbi_image_free(buffer);
}