#include "Animation.h"

void Animation::SetLinearDuration(double totalDuration) {
	uint16 size = frames.size();
	durations.clear();
	if (size == 0) return;
	if (size == 1) {
		durations.push_back(totalDuration);
		return;
	}
	double delta = totalDuration / frames.size();
	for (int i = 0; i < size; i++)
		durations.push_back(delta);
}

void Animation::AddFrame(Texture* texture, double duration) {
	frames.push_back(texture);
	durations.push_back(duration);
}

void Animation::DeleteFrame(uint16 index) {
	frames.erase(frames.begin() + index);
	durations.erase(durations.begin() + index);
}