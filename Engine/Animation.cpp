#include "Animation.h"

void Animation::SetLinearDuration(double totalDuration) {
	uint16 size = frames.size();
	if (size == 0) return;
	if (size == 1) {
		durations.push_back(totalDuration);
		return;
	}
	double delta = totalDuration / frames.size();
	for (int i = 0; i < size; i++)
		durations.push_back(delta);
}