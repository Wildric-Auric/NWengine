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
extern Image IMAGE_WARRIOR_IDLE_1;
extern Image IMAGE_WARRIOR_IDLE_2;
extern Image IMAGE_WARRIOR_IDLE_3;
extern Image IMAGE_WARRIOR_IDLE_4;
extern Image IMAGE_WARRIOR_IDLE_5;
extern Image IMAGE_WARRIOR_IDLE_6;
extern Image* IMAGES_WARRIOR_IDLE_ARRAY[6];
extern Image IMAGE_BACKGROUND;
extern Image IMAGE_TREE1;
extern Image IMAGE_TREE2;
extern Image IMAGE_BUSH1;
extern Image IMAGE_BUSH2;
extern Image IMAGE_GROUND;
void FreeBuffer(unsigned char* buffer);
void LoadImages();
