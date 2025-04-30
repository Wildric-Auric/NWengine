#include "UIManager.h"

Camera* UIManager::GetCamera() { return _camContainer.GetComponent<Camera>(); }

Camera* UIManager::GetTmpCamera() { return _lastCam; }

void UIManager::OnAdd() { _camContainer.AddComponent<Camera>(); }

void UIManager::Update() {}

void UIManager::Bind() {
	_lastCam = Camera::GetActiveCamera();
	GetCamera()->Use();
}

void UIManager::Unbind() {
	if(_lastCam)
		_lastCam->Use();
}
