#include "SceneEditor.h"

Camera*		SceneEditor::cam = nullptr;
GameObject  SceneEditor::cameraObj;

void SceneEditor::Init() {
	cameraObj = GameObject();
	SceneEditor::cam = cameraObj.AddComponent<Camera>();
}

void SceneEditor::Update() {
	SceneEditor::cam->Update();
	SceneEditor::cam->clearColor = fVec3(0.1f, 0.0f, 0.4f);
	SceneEditor::cam->Capture();
}