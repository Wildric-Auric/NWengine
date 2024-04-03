#pragma once

#include <Globals.h>
#include <string>

#include "Asset.h"


class Image : public Asset {
public:
	std::string path = "";
	uint8* pixelBuffer = nullptr;
	int channels       = 0;
	int width		   = 0;
	int height		   = 0;
	bool alpha		   = 0;
	
	Image() = default;
	Image(const std::string& x, uint8* buf, int ch, int w, int h, bool a) : path{x}, pixelBuffer{buf}, channels{ch},
	width{w}, height{h}, alpha{a} {};
	Asset* LoadFromFile(const char* path, void* output) override;
	void   Clean()                                      override;
};

