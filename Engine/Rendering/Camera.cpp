#include "Camera.h"
#include "Scene.h"
#include "Context.h"
void Camera::Capture(float r, float g, float b, float a) {	/// Captures  current scene (see currentScene variable in Scene class)


	this->fbo.Bind();
		Context::Clear(r,g,b,a);
		Camera* temp = ActiveCamera;
		ActiveCamera = this;
		Scene::currentScene->Draw();
		ActiveCamera = temp;
	this->fbo.Unbind();

}



Camera::Camera(GameObject* go) {
	size = iVec2(Globals::NATIVE_WIDTH, Globals::NATIVE_HEIGHT);
	projectionMatrix = glm::ortho(-(float)Globals::NATIVE_WIDTH / 2.0f, (float)Globals::NATIVE_WIDTH / 2.0f, 
								  -(float)Globals::NATIVE_HEIGHT / 2.0f, (float)Globals::NATIVE_HEIGHT / 2.0f);
	position = Vector2<int>(0, 0);
	fbo = FrameBuffer(&Texture::resList["Camera " + std::to_string(go->id)]);
};

void Camera::ChangeOrtho(float minX, float maxX, float minY, float maxY) {
	projectionMatrix = glm::ortho(minX, maxX, minY, maxY);
}

Camera* Camera::ActiveCamera = nullptr;
std::map<GameObject*, Camera> Camera::componentList;

void Camera::Update() {
		//TODO::Optimize this update
		viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(float)position.x, -(float)position.y, 0.0f));
		viewMatrix = glm::scale(viewMatrix, glm::vec3(zoom, zoom, 1.0f));
		viewMatrix = glm::rotate(viewMatrix, DegToRad(rotation), glm::vec3(0,0,1));
}

void Camera::MoveTo(Vector2<int> target, float targetTime) {
	position = target;   //No interpolation yet sincde no generator UPDATE 06/2022:: Generators??? Are you kiding
}