#include "RuntimeManager.h"
#include "Camera.h"
#include "Scene.h"

EngineMode RuntimeManager::__currentMode = EngineMode::EDIT_MODE;
Scene* RuntimeManager::scene = nullptr;


int RuntimeManager::switchMode(EngineMode newMode) {
	if (newMode == __currentMode)
		return 0;
	__currentMode = newMode;
	if (newMode == EngineMode::PLAY_MODE) {
		RuntimeManager::scene = Scene::currentScene;
		if (Scene::currentScene == nullptr)
			return 1;
		Scene::currentScene = new Scene(RuntimeManager::scene->name);
		Scene::currentScene->LoadScene();
		Scene::currentScene->Start();
		return 1;
	}

	if (newMode == EngineMode::EDIT_MODE) {
		delete Scene::currentScene;
		Camera::ActiveCamera = nullptr;
		Scene::currentScene = RuntimeManager::scene;
		Scene::currentScene->SetUp();
		return 1;
	}

	return 0;
}
