#pragma once
#include <vector>
#include <string>
#include "Globals.h"
#include "Texture.h"

class Animation {
public:
	std::vector<Texture*> frames;
	std::vector<double> durations; //What if we switch double to uint16 so that it's frames
	bool repeat = 0;

	void AddFrame(Texture* texture, double duration);
	void DeleteFrame(uint16 index);
	void SetLinearDuration(double totalDuration);
};

