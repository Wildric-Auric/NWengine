#include "Animator.h"

Animator::Animator(GameObject* go) {
	attachedObj = go;
}

void Animator::Animate(Sprite* buffer) {
	//TODO::Set first time the frame with 0 index
	if (!isActive) return;
	cycle += Globals::deltaTime;
	double duration = animation.durations[currentFrame];
	if (cycle >= duration) {
		currentFrame = (currentFrame + 1) % animation.frames.size(); 
		//TODO::Error check; sprite not existing
		buffer->SetTexture(animation.frames[currentFrame]->name);
		cycle = 0.0;
		if (!animation.repeat && (currentFrame == animation.frames.size() - 1)) isActive = false;
	}
}
