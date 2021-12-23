#pragma once


class Image {
public:
	Image(const char* path = "Ressources/test.png", bool alpha = 1);
	unsigned char* tex;
	int width;
	int height;
	int channels;
	bool alpha;
};
extern Image TEX1;
void freeBuffer(unsigned char* buffer);
void loadImages();
