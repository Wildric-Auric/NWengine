
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb/stb_image.h"
#include <string>
#include <fstream>
#include <vector>
#include "RessourcesLoader.h"



std::map<int, GameObject*> allObjects;

std::map<std::string, GameObject> objects;
std::map<std::string, Image> images;
std::map<std::string, Texture> textures;
std::map<std::string, Shader> shaders;

unsigned char* behindPixels        =    new unsigned char[4*ORIGINAL_WIDTH*ORIGINAL_HEIGHT];




Image::Image(const char* path, bool alpha) {
	stbi_set_flip_vertically_on_load(1);
	alpha                             =    alpha;
	tex                               =    stbi_load(path,&width,&height,&channels,3+alpha);
};

void LoadRessources() {
	std::ifstream file("Engine Data/Ressources Data/data.txt");
	textures["grabTex"] = Texture(ORIGINAL_WIDTH, ORIGINAL_HEIGHT, behindPixels);
	if (file) {
		std::vector<std::string> state;
		/*

		*/
		for (std::string line; std::getline(file, line);) {
			std::string key = "";
			bool keyPassed = false;
			for (int i = 0; i <= line.size(); i++) {

				char currentChar = line[i];

				//Getting key and setting state
				if (i == line.size()) {
					if (key == "end") {
						state.pop_back();
						continue;
					}
				}
				if (currentChar == ' ' || currentChar == '\t') continue;

				if (currentChar == ':') {
					keyPassed = true;
					state.push_back(key);
					key = "";
					continue;
				}

				key += currentChar;
				if (!keyPassed) continue;

				//Dealing with values if any
				if (currentChar == '{') {
					key = "";
					std::string arg = "";
					std::vector<std::string> args;
					i += 1;
					//parsing arguments
					int argNum = 0;
					while (1) {
						currentChar = line[i];
						i += 1;
						if (currentChar == ',' || currentChar == '}') {
							args.push_back(arg);
							arg = "";
							if (currentChar == '}') break;
							continue;
						}
						arg += currentChar;
					}



					if (state[state.size() - 2] == "Images")
						images[state[state.size() - 1].substr(1, state[state.size() - 1].size() - 2)] = Image(args[0].c_str(), stoi(args[1]));

					else if (state[state.size() - 2] == "Textures") {
						textures[state[state.size() - 1].substr(1, state[state.size() - 1].size() - 2)] = Texture(images[args[0]].width, images[args[0]].height, images[args[0]].tex, stoi(args[1]), stoi(args[2]));
					}

					else if (state[state.size() - 2] == "Shaders")
						shaders[state[state.size() - 1].substr(1, state[state.size() - 1].size() - 2)] = Shader(args[0].c_str());

					else if (state[state.size() - 2] == "GameObjects")
						objects[state[state.size() - 1].substr(1, state[state.size() - 1].size() - 2)] = GameObject(&textures[args[0]], iVec2(0, 0), fVec2(1, 1),
							state[state.size() - 1].substr(1, state[state.size() - 1].size() - 2), &shaders[args[1]]);


					state.pop_back();
					if (i >= line.size()) break;
				}
			}
		}
	}

	shaders["shader_postProcessing"] = Shader("Shaders/PostProcessing.shader");
	objects["postProcessing"] = GameObject(&textures["grabTex"], Vector2<int>(0, 0), Vector2<float>(1.0f, 1.0f), "postProcessing", &shaders["shader_postProcessing"]);

};

void Save() {
	;
}
 
			  

void FreeBuffer(unsigned char* buffer) {
	stbi_image_free(buffer);
};