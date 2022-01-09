//TODO::Use map structure for ressources

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"
#include <string>
#include "RessourcesLoader.h"


std::map<int, GameObject*> allObjects;

std::map<const char*, GameObject> objects;
std::map<const char*, Image> images;
std::map<const char*, Texture> textures;
std::map<const char*, Shader> shaders;

unsigned char* behindPixels        =    new unsigned char[4*SCREEN_WIDTH*SCREEN_HEIGHT];

const char* path                   =    "Ressources/Images/img2.png";



Image* IMAGES_WARRIOR_IDLE_ARRAY[6];
Image* IMAGES_WARRIOR_RUN_ARRAY[8];     //TODO::Change this temporary solution


Image::Image(const char* path, bool alpha) {
	stbi_set_flip_vertically_on_load(1);
	alpha                             =    alpha;
	tex                               =    stbi_load(path,&width,&height,&channels,3+alpha);
};

void LoadImages() {
	images["IMAGE_APPLE"]			            =    Image(path,true);
	images["IMAGE_WARRIOR_IDLE_1"]	            =    Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_1.png",true);
	images["IMAGE_WARRIOR_IDLE_2"]	            =    Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_2.png",1);
	images["IMAGE_WARRIOR_IDLE_3"]	            =    Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_3.png",1);
	images["IMAGE_WARRIOR_IDLE_4"]	            =    Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_4.png",1);
	images["IMAGE_WARRIOR_IDLE_5"]	            =    Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_5.png",1);
	images["IMAGE_WARRIOR_IDLE_6"]	            =    Image("Ressources/Images/Warrior/Individual Sprite/idle/Warrior_Idle_6.png",1);
	images["IMAGE_BACKGROUND"]				    =    Image("Ressources/Images/Forrest_Tileset/BG/NonParallax.png",1);
	images["IMAGE_TREE1"]					    =    Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0021_Layer-22.png",1);
	images["IMAGE_TREE2"]					    =    Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0022_Layer-23.png",1);
	images["IMAGE_BUSH1"]					    =    Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0000_Layer-1.png",1);
	images["IMAGE_BUSH2"]			            =    Image("Ressources/Images/Forrest_Tileset/Objects/Sliced/obj_0001_Layer-2.png",1);
	images["IMAGE_GROUND"]			            =    Image("Ressources/Images/ground.png",1);
	images["IMAGE_TILE1"]			            =    Image("Ressources/Images/Interior Wall.png",1);
    images["IMAGE_GROUND_TILE0"]		        =    Image("Ressources/Images/Forrest_Tileset/Tiles/cute/sprite_0.png", 1);
	images["IMAGE_GROUND_TILE1"]	            =    Image("Ressources/Images/Forrest_Tileset/Tiles/cute/sprite_6.png", 1);
	images["IMAGE_GROUND_TILE2"]	            =    Image("Ressources/Images/Forrest_Tileset/Tiles/cute/sprite_12.png", 1);
	images["IMAGE_WARRIOR_RUN_1"]               =    Image("Ressources/Images/Warrior/Individual Sprite/Run/Warrior_Run_1.png");
	images["IMAGE_WARRIOR_RUN_2"]               =    Image("Ressources/Images/Warrior/Individual Sprite/Run/Warrior_Run_2.png");
	images["IMAGE_WARRIOR_RUN_3"]               =    Image("Ressources/Images/Warrior/Individual Sprite/Run/Warrior_Run_3.png");
	images["IMAGE_WARRIOR_RUN_4"]               =    Image("Ressources/Images/Warrior/Individual Sprite/Run/Warrior_Run_4.png");
	images["IMAGE_WARRIOR_RUN_5"]               =    Image("Ressources/Images/Warrior/Individual Sprite/Run/Warrior_Run_5.png");
	images["IMAGE_WARRIOR_RUN_6"]               =    Image("Ressources/Images/Warrior/Individual Sprite/Run/Warrior_Run_6.png");
	images["IMAGE_WARRIOR_RUN_7"]               =    Image("Ressources/Images/Warrior/Individual Sprite/Run/Warrior_Run_7.png");
	images["IMAGE_WARRIOR_RUN_8"]               =    Image("Ressources/Images/Warrior/Individual Sprite/Run/Warrior_Run_8.png");
	
		

	IMAGES_WARRIOR_IDLE_ARRAY[0] = &images["IMAGE_WARRIOR_IDLE_1"];
	IMAGES_WARRIOR_IDLE_ARRAY[1] = &images["IMAGE_WARRIOR_IDLE_2"];
	IMAGES_WARRIOR_IDLE_ARRAY[2] = &images["IMAGE_WARRIOR_IDLE_3"];
	IMAGES_WARRIOR_IDLE_ARRAY[3] = &images["IMAGE_WARRIOR_IDLE_4"];
	IMAGES_WARRIOR_IDLE_ARRAY[4] = &images["IMAGE_WARRIOR_IDLE_5"];
	IMAGES_WARRIOR_IDLE_ARRAY[5] = &images["IMAGE_WARRIOR_IDLE_6"];

	IMAGES_WARRIOR_RUN_ARRAY[0]  = &images["IMAGE_WARRIOR_RUN_1"];
	IMAGES_WARRIOR_RUN_ARRAY[1] = &images["IMAGE_WARRIOR_RUN_2"];
	IMAGES_WARRIOR_RUN_ARRAY[2] = &images["IMAGE_WARRIOR_RUN_3"];
	IMAGES_WARRIOR_RUN_ARRAY[3] = &images["IMAGE_WARRIOR_RUN_4"];
	IMAGES_WARRIOR_RUN_ARRAY[4] = &images["IMAGE_WARRIOR_RUN_5"];
	IMAGES_WARRIOR_RUN_ARRAY[5] = &images["IMAGE_WARRIOR_RUN_6"];
	IMAGES_WARRIOR_RUN_ARRAY[6] = &images["IMAGE_WARRIOR_RUN_7"];
	IMAGES_WARRIOR_RUN_ARRAY[7] = &images["IMAGE_WARRIOR_RUN_8"];




};

void LoadTextures() {
	textures["tex"]                              =    Texture(images["IMAGE_APPLE"].width, images["IMAGE_APPLE"].height, images["IMAGE_APPLE"].tex, 1, 0);
	textures["grabTex"             ]             =    Texture(SCREEN_WIDTH, SCREEN_HEIGHT, behindPixels);
	textures["warriorTex"          ]             =    Texture(images["IMAGE_WARRIOR_IDLE_1"].width, images["IMAGE_WARRIOR_IDLE_1"].height, images["IMAGE_WARRIOR_IDLE_1"].tex, 1, 0);
	textures["backgroundTex"       ]             =    Texture(images["IMAGE_BACKGROUND"].width, images["IMAGE_BACKGROUND"].height, images["IMAGE_BACKGROUND"].tex, 1, 1);
	textures["bush1Tex"            ]             =    Texture(images["IMAGE_BUSH1"].width, images["IMAGE_BUSH1"].height, images["IMAGE_BUSH1"].tex, 1, 0);
	textures["bush2Tex"            ]             =    Texture(images["IMAGE_BUSH2"].width, images["IMAGE_BUSH2"].height, images["IMAGE_BUSH2"].tex, 1, 0);
	textures["tree1Tex"            ]             =    Texture(images["IMAGE_TREE1"].width, images["IMAGE_TREE1"].height, images["IMAGE_TREE1"].tex, 1, 0);
	textures["tree2Tex"            ]             =    Texture(images["IMAGE_TREE2"].width, images["IMAGE_TREE2"].height, images["IMAGE_TREE2"].tex, 1, 0);
	textures["groundTex"           ]             =    Texture(images["IMAGE_GROUND"].width, images["IMAGE_GROUND"].height, images["IMAGE_GROUND"].tex, 1, 0);
	textures["tile1Tex"]					     =    Texture(images["IMAGE_TILE1"].width, images["IMAGE_TILE1"].height, images["IMAGE_TILE1"].tex, 1, 1);
	textures["texture_groundTile0"]              =    Texture(images["IMAGE_GROUND_TILE0"].width, images["IMAGE_GROUND_TILE0"].height, images["IMAGE_GROUND_TILE0"].tex, 1, 1);
	textures["texture_groundTile1"]              =    Texture(images["IMAGE_GROUND_TILE1"].width, images["IMAGE_GROUND_TILE1"].height, images["IMAGE_GROUND_TILE1"].tex, 1, 1);
	textures["texture_groundTile2"]              =    Texture(images["IMAGE_GROUND_TILE2"].width, images["IMAGE_GROUND_TILE2"].height, images["IMAGE_GROUND_TILE2"].tex, 1, 1);

};
void LoadShaders() {

	shaders["shader_default"]                   =    Shader("Shaders/Shader1.shader");
	shaders["shader_lightSurface"]              =    Shader("Shaders/LightSurface.shader");
	shaders["shader_grabPass"]                  =    Shader("Shaders/GrabPass.shader");
	shaders["shader_postProcessing"]            =    Shader("Shaders/PostProcessing.shader");
	shaders["shader_simple"]                    =    Shader("Shaders/Line.shader");
	shaders["shader_simple1"]                   =    Shader("Shaders/Line.shader");
	shaders["shader_text"]                      =    Shader("Shaders/Text.shader");


};

void LoadGameObjects() {
	
	objects["lesbeanApple"]		         =    GameObject(&textures["tex"], Vector2<int>(0, 0), Vector2<float>(0.5f, 0.5f), &objects["lesbeanApple"],&shaders["shader_default"]);					//Quad(Vector2<int>(0 ,0 ), 300.0F, 300.0F);
	objects["lesbeanApple2"]	         =    GameObject(&textures["tex"], Vector2<int>(200, 100), Vector2<float>(-.5f, .5f), &objects["lesbeanApple2"],&shaders["shader_default"]);
	objects["grabPass"]			         =    GameObject(&textures["grabTex"], Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f), &objects["grabPass"],&shaders["shader_grabPass"]);
	objects["lightSurface"]		         =    GameObject(&textures["grabTex"], Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f), &objects["lightSurface"],&shaders["shader_lightSurface"]);
	objects["postProcessing"]	         =    GameObject(&textures["grabTex"], Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f), &objects["postProcessing"],&shaders["shader_postProcessing"]);
	objects["warrior"]					 =    GameObject(&textures["warriorTex"], Vector2<int>(0, -12), Vector2<float>(1.0f, 1.0f), &objects["warrior"]);
	float s =    2.2;
	objects["background"]	         =    GameObject(&textures["backgroundTex"], Vector2<int>(-337, 130), Vector2<float>(s, s), &objects["background"]);
	objects["background1"]	         =    GameObject(&textures["backgroundTex"], Vector2<int>(-337 +		objects["background"].size.x, 130), Vector2<float>(s, s),		&objects["background1"]	);
	objects["background2"]	         =    GameObject(&textures["backgroundTex"], Vector2<int>(-337 + 2 * objects["background"].size.x, 130), Vector2<float>(s, s), &objects["background2"]	);
	objects["background3"]	         =    GameObject(&textures["backgroundTex"], Vector2<int>(-337 + 3 * objects["background"].size.x, 130), Vector2<float>(s, s), &objects["background3"]	);
	objects["background4"]	         =    GameObject(&textures["backgroundTex"], Vector2<int>(-337 + 4 * objects["background"].size.x, 130), Vector2<float>(s, s), &objects["background4"]	);
	s            =    2.0f;
	objects["bush1"]	         =    GameObject(&textures["bush1Tex"], Vector2<int>(113, -28), Vector2<float>(s, s), &objects["bush1"]);
	objects["bush2"]	         =    GameObject(&textures["bush2Tex"], Vector2<int>(-119, -28), Vector2<float>(s, s), &objects["bush2"]);
	objects["tree1"]	         =    GameObject(&textures["tree1Tex"], Vector2<int>(156, 12), Vector2<float>(s, s), &objects["tree1"]);
	objects["tree2"]	         =    GameObject(&textures["tree2Tex,"], Vector2<int>(-212, 19), Vector2<float>(s, s), &objects["tree2"]);
	s           =    1.5f;
	objects["ground"]	             =    GameObject(&textures["groundTex"], Vector2<int>(-100, -40), Vector2<float>(s, s), &objects["ground"]);
	objects["wallTile0"]	         =    GameObject(&textures["tile1Tex"], Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f), &objects["wallTile0"],&shaders["shader_default"]);
	objects["groundTile0"]	         =    GameObject(&textures["texture_groundTile0"], Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f), &objects["groundTile0"]);
	objects["groundTile1"]	         =    GameObject(&textures["texture_groundTile1"], Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f), &objects["groundTile1"]);
	objects["groundTile2"]	         =    GameObject(&textures["texture_groundTile2"], Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f), &objects["groundTile2"]);

};					  

void FreeBuffer(unsigned char* buffer) {
	stbi_image_free(buffer);
};