#include "Animator.h"
#include "NWTime.h"

Animator::Animator(GameObject* go) {
	attachedObj = go;
}

void Animator::Update() {
	//TODO::Set first time the frame with 0 index
	if (!isActive) return;
	Sprite* buffer = attachedObj->GetComponent<Sprite>();
	if (buffer == nullptr ) return;
	cycle += NWTime::GetDeltaTime();
	double duration = animation.durations[currentFrame];
	if (cycle >= duration) {
		currentFrame = (currentFrame + 1) % animation.frames.size(); 
		//TODO::Error check; sprite not existing
		buffer->SetTexture(animation.frames[currentFrame]->name);
		cycle = 0.0;
		if (!animation.repeat && (currentFrame == animation.frames.size() - 1)) isActive = false;
	}
}


void Animator::SetGameObject(void* go) {
	attachedObj = (GameObject*)go;
};
void* Animator::GetGameObject() {
	return (void*)attachedObj;
};