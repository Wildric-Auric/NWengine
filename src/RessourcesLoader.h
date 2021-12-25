#pragma once


class Image {
public:
	Image(const char* path = "Ressources/Images/test.png", bool alpha = 1);
	unsigned char* tex;
	int width;
	int height;
	int channels;
	bool alpha;
};
extern Image IMAGE_APPLE;
void FreeBuffer(unsigned char* buffer);
void LoadImages();
