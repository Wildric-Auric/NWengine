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
	
	Asset* LoadFromFile(const char* path, void* output) override;
	void   Clean()                                      override;
};

