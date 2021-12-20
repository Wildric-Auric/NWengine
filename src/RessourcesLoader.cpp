#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_write_image.h"
#include "RessourcesLoader.h"
#include <string>
#include <iostream>

const char* path = "Ressources/test.png";
Image TEX1;
Image::Image(const char *path, bool alpha) {
	stbi_set_flip_vertically_on_load(1);
	alpha = alpha;
	tex = stbi_load(path, &width, &height, &channels, 3 + alpha);
}

void loadImages() {
	TEX1 = Image(path, true);
}
void freeBuffer(unsigned char* buffer) {
	stbi_image_free(buffer);
}