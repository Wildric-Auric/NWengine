#include "Camera.h"
#include "Scene.h"
#include "PostProcessing.h"
#include "imgui/imgui.h"

void Camera::Capture() {	/// Captures  current scene (see currentScene variable in Scene class)

	Context::SetViewPort(0, 0, viewPortSize.x, viewPortSize.y);
	this->fbo.Bind();
		Context::Clear(clearColor.x, clearColor.y, clearColor.z, 1.0);
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
	projectionMatrix = glm::ortho(-(float)Context::NATIVE_WIDTH / 2.0f, (float)Context::NATIVE_WIDTH / 2.0f, 
								  -(float)Context::NATIVE_HEIGHT / 2.0f, (float)Context::NATIVE_HEIGHT / 2.0f);
	position = Vector2<int>(0, 0);
	fbo = FrameBuffer();

	attachedObj = go;
};

//void Camera::ChangeOrtho(float minX, float maxX, float minY, float maxY) {
//	projectionMatrix = glm::ortho(minX, maxX, minY, maxY);
//}

void Camera::ChangeOrtho(float sizeX, float sizeY) {
	if ((sizeX == size.x) && (sizeY == size.y)) return;
	size.x         = sizeX;
	size.y         = sizeY;
	viewPortSize.x = sizeX; //TODO::viewPortSize is deprecated, delete that variable
	viewPortSize.y = sizeY;

	projectionMatrix = glm::ortho(- sizeX * 0.5f, sizeX *0.5f, -sizeY * 0.5f, sizeY * 0.5f);
}

Camera* Camera::ActiveCamera = nullptr;

#include "Sprite.h"
void Camera::Update() {
		//TODO::Optimize this update
		viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(float)position.x, -(float)position.y, 0.0f));
		viewMatrix = glm::scale(viewMatrix, glm::vec3(zoom, zoom, 1.0f));
		viewMatrix = glm::rotate(viewMatrix, DegToRad(rotation), glm::vec3(0,0,1));
}

void Camera::MoveTo(Vector2<int> target, float targetTime) {
	position = target;   //No interpolation yet sincde no generator UPDATE 06/2022:: Generators??? Are you kiding
}



void Camera::Use() {
	if (Camera::ActiveCamera != nullptr)
		Camera::ActiveCamera->isActive = 0;
	this->isActive = 1;
	Camera::ActiveCamera = this;
}
void Camera::Gui() {

	if (NWGui::CheckBox("Activate", &isActive)) {
		if (isActive) { Use(); }
		else {
			Camera::ActiveCamera = nullptr;
			this->isActive = 0;
		}
	};
		
	NWGui::DragValue("Camera Position", &position.x, ImGuiDataType_S32, 2);
	NWGui::DragValue("Camera Rotation", &rotation, ImGuiDataType_Float, 1);
	NWGui::DragValue("Camera Zoom", &zoom, ImGuiDataType_Float, 1, 0.1f, 0.0f, 100.0f);
	NWGui::DragValue("Clearing color", &clearColor.x, ImGuiDataType_Float, 3, 0.1, 0.0f, 1.0f);
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
	
	return 0;
}