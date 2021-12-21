#pragma once

class Texture {
private:
	unsigned int texture;
public:
	Texture(int width, int height, unsigned char* texRes, bool alpha = true, bool repeat = false);
	void Bind(unsigned int slot);
	void UpdateTexture(int width, int height, unsigned char* texRes,unsigned int slot, bool alpha = 1);
	//The UpdateTexture method was added so I can use glReadPixels
	//initially I recreate Texture object in each mainloop iteration but an unknown bug occurs after several seconds, and it's not optimized
	//This solution works for now
};
