#include "Camera.h"
#include "Scene.h"

fVec2 Camera::GetPosition() {
    return position;
}

fVec2 Camera::GetSize() {
    return size;
}

Camera* Camera::GetActiveCamera() {
    return Camera::ActiveCamera;
}

void Camera::UpdateActiveCamera() {
	if (Camera::ActiveCamera != nullptr) 
		Camera::ActiveCamera->Capture();
}


void Camera::_ClearAtts() {
    for (auto& p : clearCols) {
        if (p.first < 0 || p.first >= fbo.attachments.size())  continue;
        fbo.ClearAttachment(p.first, p.second);
    }
}

void Camera::Capture() {	/// Captures  current scene (see currentScene variable in Scene class)
	Context::SetViewPort(0, 0, viewPortSize.x, viewPortSize.y);
	this->fbo.Bind();
		Context::Clear(clearColor.x, clearColor.y, clearColor.z, alpha);
        _ClearAtts(); 
		Camera* temp = ActiveCamera;
		ActiveCamera = this;
		if (Scene::currentScene != nullptr)
			Scene::currentScene->Draw();
		ActiveCamera = temp;
	this->fbo.Unbind();
}

Camera::Camera(GameObject* go) {
	attachedObject = go;
	position = Vector2<int>(0, 0);
};

void Camera::ChangeOrthoWithMSAA(float sizeX, float sizeY, MSAAValue msaa) {
	if ((sizeX == size.x) && (sizeY == size.y) && (this->_msaa == msaa)) return;
	size.x         = sizeX;
	size.y         = sizeY;
	viewPortSize.x = sizeX;
	viewPortSize.y = sizeY;
	_msaa = msaa;

	fbo.Delete();
	fbo.SetUp(size, msaa);
	OrthorgraphicMat(projectionMatrix, -sizeX * 0.5f, sizeX * 0.5f, -sizeY * 0.5f, sizeY * 0.5f);
}

void Camera::ChangeOrtho(float sizeX, float sizeY) {
	ChangeOrthoWithMSAA(sizeX, sizeY, this->_msaa);
}


Camera* Camera::ActiveCamera = nullptr;

void Camera::Update() {
    //TODO::Optimize this update
    viewMatrix = Matrix4<float>(1.0);
    ScaleMat(viewMatrix, fVec3(zoom, zoom, 1.0));
    RotateMat(viewMatrix, rotation, fVec3(0.0f, 0.0f, 1.0f));
    TranslateMat(viewMatrix, fVec3(-position.x, -position.y, 0.0f));
}

void Camera::MoveTo(Vector2<int> target, float targetTime) {
	position = target; 
}

void Camera::SetClearColor(const fVec4& color) { 
    clearColor = *((fVec3*)(&color.x));
    alpha = color.a;
}

void Camera::SetClearColor(int i, const fVec4& color) {
    clearCols[i] = color;
}

void Camera::ResetClearColors() {
    clearCols.clear(); 
}


void Camera::Use() {
	if (Camera::ActiveCamera != nullptr)
		Camera::ActiveCamera->isActive = 0;
	this->isActive = 1;
	Camera::ActiveCamera = this;
}


Camera::~Camera() {
	this->fbo.Delete();
	if (Camera::ActiveCamera == this)
		Camera::ActiveCamera = nullptr;
}
