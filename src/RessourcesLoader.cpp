//TODO::Use map structure for ressources

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"
#include <string>
#include "RessourcesLoader.h"


unsigned char* behindPixels = new unsigned char[4 * SCREEN_WIDTH * SCREEN_HEIGHT];
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
Image IMAGE_TILE1;


Texture* tex             = (Texture*)malloc(sizeof(Texture));
Texture* grabTex		 = (Texture*)malloc(sizeof(Texture));
Texture* warriorTex		 = (Texture*)malloc(sizeof(Texture));
Texture* backgroundTex	 = (Texture*)malloc(sizeof(Texture));
Texture* bush1Tex		 = (Texture*)malloc(sizeof(Texture));
Texture* bush2Tex		 = (Texture*)malloc(sizeof(Texture));
Texture* tree1Tex		 = (Texture*)malloc(sizeof(Texture));
Texture* tree2Tex		 = (Texture*)malloc(sizeof(Texture));
Texture* groundTex		 = (Texture*)malloc(sizeof(Texture));
Texture* tile1Tex        = (Texture*)malloc(sizeof(Texture));


Shader* shader_default        =		(Shader*)malloc(sizeof(Shader));
Shader* shader_lightSurface   =		(Shader*)malloc(sizeof(Shader));
Shader* shader_grabPass       =		(Shader*)malloc(sizeof(Shader));
Shader* shader_postProcessing =		(Shader*)malloc(sizeof(Shader));
Shader* shader_simple         =		(Shader*)malloc(sizeof(Shader));


GameObject* lesbeanApple = (GameObject*)malloc(sizeof(GameObject));
GameObject* lesbeanApple2 = (GameObject*)malloc(sizeof(GameObject));
GameObject* grabPass = (GameObject*)malloc(sizeof(GameObject));
GameObject* lightSurface = (GameObject*)malloc(sizeof(GameObject));
GameObject* postProcessing = (GameObject*)malloc(sizeof(GameObject));
GameObject* warrior = (GameObject*)malloc(sizeof(GameObject));
GameObject* background = (GameObject*)malloc(sizeof(GameObject));
GameObject* background1 = (GameObject*)malloc(sizeof(GameObject));
GameObject* background2 = (GameObject*)malloc(sizeof(GameObject));
GameObject* background3 = (GameObject*)malloc(sizeof(GameObject));
GameObject* background4 = (GameObject*)malloc(sizeof(GameObject));
GameObject* tree1 = (GameObject*)malloc(sizeof(GameObject));
GameObject* tree2 = (GameObject*)malloc(sizeof(GameObject));
GameObject* bush1 = (GameObject*)malloc(sizeof(GameObject));
GameObject* bush2 = (GameObject*)malloc(sizeof(GameObject));
GameObject* ground = (GameObject*)malloc(sizeof(GameObject));
GameObject* tile1Obj = (GameObject*)malloc(sizeof(GameObject));


Image::Image(const char* path, bool alpha) {
	stbi_set_flip_vertically_on_load(1);
	alpha = alpha;
	tex = stbi_load(path, &width, &height, &channels, 3 + alpha);
};

void LoadImages() {
	IMAGE_APPLE = Image(path, true);
	IMAGE_WARRIOR_IDLE_1 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_1.png", true);
	IMAGE_WARRIOR_IDLE_2 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_2.png",1);
	IMAGE_WARRIOR_IDLE_3 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_3.png",1);
	IMAGE_WARRIOR_IDLE_4 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_4.png",1);
	IMAGE_WARRIOR_IDLE_5 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_5.png",1);
	IMAGE_WARRIOR_IDLE_6 = Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_6.png",1);
	IMAGE_BACKGROUND     = Image("Ressources/Images/Forrest_Tileset/BG/NonParallax.png",1);
	IMAGE_TREE1 =		   Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0021_Layer-22.png", 1);
	IMAGE_TREE2 =		   Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0022_Layer-23.png", 1);
	IMAGE_BUSH1 =		   Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0000_Layer-1.png", 1);
	IMAGE_BUSH2 =		   Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0001_Layer-2.png", 1);
	IMAGE_GROUND	=	   Image("Ressources/Images/ground.png", 1);
	IMAGE_TILE1          = Image("Ressources/Images/Interior Wall.png", 1);

};

void LoadTextures() {
	*tex = Texture(IMAGE_APPLE.width, IMAGE_APPLE.height, IMAGE_APPLE.tex, 1, 0);
	*grabTex = Texture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels);
	*warriorTex = Texture(IMAGE_WARRIOR_IDLE_1.width, IMAGE_WARRIOR_IDLE_1.height, IMAGE_WARRIOR_IDLE_1.tex, 1, 0);
	*backgroundTex = Texture(IMAGE_BACKGROUND.width, IMAGE_BACKGROUND.height, IMAGE_BACKGROUND.tex, 1, 1);
	*bush1Tex = Texture(IMAGE_BUSH1.width, IMAGE_BUSH1.height, IMAGE_BUSH1.tex, 1, 0);
	*bush2Tex = Texture(IMAGE_BUSH2.width, IMAGE_BUSH2.height, IMAGE_BUSH2.tex, 1, 0);
	*tree1Tex = Texture(IMAGE_TREE1.width, IMAGE_TREE1.height, IMAGE_TREE1.tex, 1, 0);
	*tree2Tex = Texture(IMAGE_TREE2.width, IMAGE_TREE2.height, IMAGE_TREE2.tex, 1, 0);
	*groundTex = Texture(IMAGE_GROUND.width, IMAGE_GROUND.height, IMAGE_GROUND.tex, 1, 0);
	*tile1Tex = Texture(IMAGE_TILE1.width, IMAGE_TILE1.height, IMAGE_TILE1.tex, 1, 1);
};
void LoadShaders() {

	*shader_default        =			Shader("Shaders/Shader1.shader");
	*shader_lightSurface   =			Shader("Shaders/LightSurface.shader");
	*shader_grabPass       =			Shader("Shaders/GrabPass.shader");
	*shader_postProcessing =			Shader("Shaders/PostProcessing.shader");
	*shader_simple		   =			Shader("Shaders/Line.shader");

};

void LoadGameObjects() {
	*lesbeanApple =			GameObject(tex, Vector2<int>(0, 0), Vector2<float>(0.5f, 0.5f), shader_default);					//Quad(Vector2<int>(0 ,0 ), 300.0F, 300.0F);
	*lesbeanApple2 =		GameObject(tex, Vector2<int>(200, 100), Vector2<float>(-.5f, .5f), shader_default);
	*grabPass =				GameObject(grabTex, Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f),shader_grabPass);
	*lightSurface =			GameObject(grabTex, Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f),shader_lightSurface);
	*postProcessing =		GameObject(grabTex, Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f), shader_postProcessing);
	*warrior =				GameObject(warriorTex, Vector2<int>(0, -20), Vector2<float>(1.0f, 1.0f));
	float s = 2.2;
	*background =			GameObject(backgroundTex, Vector2<int>(-337, 130), Vector2<float>(s, s));
	*background1 =			GameObject(backgroundTex, Vector2<int>(-337 + background->size.x, 130), Vector2<float>(s, s));
	*background2 =			GameObject(backgroundTex, Vector2<int>(-337 + 2 * background->size.x, 130), Vector2<float>(s, s));
	*background3 =			GameObject(backgroundTex, Vector2<int>(-337 + 3 * background->size.x, 130), Vector2<float>(s, s));
	*background4 =			GameObject(backgroundTex, Vector2<int>(-337 + 4 * background->size.x, 130), Vector2<float>(s, s));
	s = 2.0f;
	*bush1 =				GameObject(bush1Tex, Vector2<int>(113, -28), Vector2<float>(s, s));
	*bush2 =				GameObject(bush2Tex, Vector2<int>(-119, -28), Vector2<float>(s, s));
	*tree1 =				GameObject(tree1Tex, Vector2<int>(156, 12), Vector2<float>(s, s));
	*tree2 =				GameObject(tree2Tex, Vector2<int>(-212, 19), Vector2<float>(s, s));
	s = 1.5f;
	*ground =				GameObject(groundTex, Vector2<int>(-100, -40), Vector2<float>(s, s));
	*tile1Obj=				GameObject(tile1Tex, Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f));
};

void FreeBuffer(unsigned char* buffer) {
	stbi_image_free(buffer);
};