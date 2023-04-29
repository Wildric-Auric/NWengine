#include "SoundPlayer.h"
#include "Inputs.h"
#include "Transform.h"
#include "Components.h"

#include "Scene.h"

void SoundPlayer::Update() {
	AudioEmitter* ae = Scene::currentScene->GetGameObject("character")->GetComponent<AudioEmitter>();
	if (ae == nullptr) {
		Console::Write("Cannot find AudioEmitter");
		return;
	}
	if (Inputs::n_0 && !ae->sound->isPlaying)
		ae->sound->Play();
	if (Inputs::n_1 && ae->sound->isPlaying)
		ae->sound->Stop();
}

Scriptable* SoundPlayer::GetScript(GameObject* goc) {
	return new SoundPlayer(goc);
};