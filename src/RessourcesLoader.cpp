//TODO::Use map structure for ressources

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"
#include <string>
#include <iostream>

#include "RessourcesLoader.h"

const char* path = "Ressources/Images/img2.png";
Image IMAGE_APPLE;
Image IMAGE_WARRIOR_IDLE_1;
Image IMAGE_WARRIOR_IDLE_2;
Image IMAGE_WARRIOR_IDLE_3;
Image IMAGE_WARRIOR_IDLE_4;
Image IMAGE_WARRIOR_IDLE_5;
Image IMAGE_WARRIOR_IDLE_6;

Image* IMAGES_WARRIOR_IDLE_ARRAY[6] = {
	&IMAGE_WARRIOR_IDLE_1,
	&IMAGE_WARRIOR_IDLE_2,
	&IMAGE_WARRIOR_IDLE_3,
	&IMAGE_WARRIOR_IDLE_4,
	&IMAGE_WARRIOR_IDLE_5,
	&IMAGE_WARRIOR_IDLE_6,
};

Image IMAGE_BACKGROUND;
Image IMAGE_TREE1;
Image IMAGE_TREE2;
Image IMAGE_BUSH1;
Image IMAGE_BUSH2;
Image IMAGE_GROUND;

Image::Image(const char *path, bool alpha) {
	stbi_set_flip_vertically_on_load(1);
	alpha = alpha;
	tex = stbi_load(path, &width, &height, &channels, 3 + alpha);
}

void LoadImages() {
	IMAGE_APPLE = Image(path, true);
	IMAGE_WARRIOR_IDLE_1 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_1.png", true);
	IMAGE_WARRIOR_IDLE_2 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_2.png",1);
	IMAGE_WARRIOR_IDLE_3 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_3.png",1);
	IMAGE_WARRIOR_IDLE_4 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_4.png",1);
	IMAGE_WARRIOR_IDLE_5 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_5.png",1);
	IMAGE_WARRIOR_IDLE_6 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_6.png",1);
	IMAGE_BACKGROUND     = Image("Ressources/Images/Forrest_Tileset/BG/NonParallax.png",1);
	IMAGE_TREE1 = Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0021_Layer-22.png", 1);
	IMAGE_TREE2 = Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0022_Layer-23.png", 1);
	IMAGE_BUSH1 = Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0000_Layer-1.png", 1);
	IMAGE_BUSH2 = Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0001_Layer-2.png", 1);
	IMAGE_GROUND = Image("Ressources/Images/ground.png", 1);

}
void FreeBuffer(unsigned char* buffer) {
	stbi_image_free(buffer);
}