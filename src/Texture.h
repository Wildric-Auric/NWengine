#pragma once

class Texture {
private:
	unsigned int texture;
public:
	Texture(int width, int height, unsigned char* texRes, bool alpha = true, bool repeat = false);
	void Bind(unsigned int slot);
};
