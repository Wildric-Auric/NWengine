#include "Camera.h"
#include "Scene.h"
#include "imgui/imgui.h"


void Camera::UpdateActiveCamera() {
	if (Camera::ActiveCamera != nullptr) 
		Camera::ActiveCamera->Capture();
}

void Camera::Capture() {	/// Captures  current scene (see currentScene variable in Scene class)
	Context::SetViewPort(0, 0, viewPortSize.x, viewPortSize.y);
	this->fbo.Bind();
		Context::Clear(clearColor.x, clearColor.y, clearColor.z, alpha);
		Camera* temp = ActiveCamera;
		ActiveCamera = this;
		if (Scene::currentScene != nullptr)
			Scene::currentScene->Draw();
		ActiveCamera = temp;
	this->fbo.Unbind();
}

Camera::Camera(GameObject* go) {
	size = iVec2(Context::NATIVE_WIDTH, Context::NATIVE_HEIGHT);
	viewPortSize = size;
	OrthorgraphicMat(projectionMatrix,  -(float)Context::NATIVE_WIDTH / 2.0f, (float)Context::NATIVE_WIDTH / 2.0f,
									    -(float)Context::NATIVE_HEIGHT / 2.0f, (float)Context::NATIVE_HEIGHT / 2.0f);
	position = Vector2<int>(0, 0);
	fbo = FrameBuffer();

	attachedObj = go;
};

void Camera::ChangeOrtho(float sizeX, float sizeY) {
	if ((sizeX == size.x) && (sizeY == size.y)) return;
	size.x         = sizeX;
	size.y         = sizeY;

	viewPortSize.x = sizeX;
	viewPortSize.y = sizeY;
	fbo.Delete();
	fbo = FrameBuffer(sizeX, sizeY);

	OrthorgraphicMat(projectionMatrix, -sizeX * 0.5f, sizeX * 0.5f, -sizeY * 0.5f, sizeY * 0.5f);
}

void Camera::SetFilteringData(const TextureDataUpdate& texData) {
		fbo.RenderedImage.UpdateTextureData(texData);
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

int Camera::Serialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	WRITE_ON_BIN(data, "Camera", 6, sizeBuffer);
	WRITE_ON_BIN(data, &isActive, sizeof(isActive), sizeBuffer);
	WRITE_ON_BIN(data, &position.x, sizeof(position.x), sizeBuffer);
	WRITE_ON_BIN(data, &position.y, sizeof(position.y), sizeBuffer);
	WRITE_ON_BIN(data, &rotation, sizeof(rotation), sizeBuffer);
	WRITE_ON_BIN(data, &zoom, sizeof(zoom), sizeBuffer);
	WRITE_ON_BIN(data, &clearColor.x, sizeof(clearColor.x), sizeBuffer);
	WRITE_ON_BIN(data, &clearColor.y, sizeof(clearColor.y), sizeBuffer);
	WRITE_ON_BIN(data, &clearColor.z, sizeof(clearColor.z), sizeBuffer);
	WRITE_ON_BIN(data, &size.x,       sizeof(size.x),		sizeBuffer);
	WRITE_ON_BIN(data, &size.y,       sizeof(size.y),		sizeBuffer);

	return 0;
}

int Camera::Deserialize(std::fstream* data, int offset) {
	int sizeBuffer = 0;
	READ_FROM_BIN(data, &isActive, sizeBuffer);
	if (isActive) Use();
	READ_FROM_BIN(data, &position.x, sizeBuffer);
	READ_FROM_BIN(data, &position.y, sizeBuffer);
	READ_FROM_BIN(data, &rotation, sizeBuffer);
	READ_FROM_BIN(data, &zoom, sizeBuffer);
	READ_FROM_BIN(data, &clearColor.x, sizeBuffer);
	READ_FROM_BIN(data, &clearColor.y, sizeBuffer);
	READ_FROM_BIN(data, &clearColor.z, sizeBuffer);
	READ_FROM_BIN(data, &viewPortSize.x,       sizeBuffer);
	READ_FROM_BIN(data, &viewPortSize.y,       sizeBuffer);
	ChangeOrtho(viewPortSize.x, viewPortSize.y);
	return 0;
}