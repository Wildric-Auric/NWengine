#include "Camera.h"
#include "Scene.h"
#include "PostProcessing.h"
#include "Console.h"

void Camera::Capture(float r, float g, float b, float a) {	/// Captures  current scene (see currentScene variable in Scene class)

	Context::SetViewPort(0, 0, viewPortSize.x, viewPortSize.y);
	this->fbo.Bind();
		Context::Clear(r,g,b,a);
		Camera* temp = ActiveCamera;
		ActiveCamera = this;
		Scene::currentScene->Draw();
		ActiveCamera = temp;
	this->fbo.Unbind();

}

void Camera::CaptureWithPostProcessing(void* pp, float r, float g, float b, float a) {
	PostProcessing* pp2 = (PostProcessing*)pp;
	pp2->fbo.Bind();
		Context::Clear(r, g, b, a);
		Camera* temp = ActiveCamera;
		ActiveCamera = this;
		pp2->renderQuad.Draw();
		ActiveCamera = temp;
	pp2->fbo.Unbind();
};

Camera::Camera(GameObject* go) {
	size = iVec2(Context::NATIVE_WIDTH, Context::NATIVE_HEIGHT);
	viewPortSize = size;
	projectionMatrix = glm::ortho(-(float)Context::NATIVE_WIDTH / 2.0f, (float)Context::NATIVE_WIDTH / 2.0f, 
								  -(float)Context::NATIVE_HEIGHT / 2.0f, (float)Context::NATIVE_HEIGHT / 2.0f);
	position = Vector2<int>(0, 0);
	fbo = FrameBuffer();

	attachedObj = go;
};

void Camera::ChangeOrtho(float minX, float maxX, float minY, float maxY) {
	projectionMatrix = glm::ortho(minX, maxX, minY, maxY);
}

void Camera::ChangeOrtho(float sizeX, float sizeY) {
	projectionMatrix = glm::ortho(-sizeX / 2.0, sizeX / 2.0, -sizeY / 2.0, sizeY / 2.0);
}

Camera* Camera::ActiveCamera = nullptr;

#include "Sprite.h"
void Camera::Update() {
		//TODO::Optimize this update
		viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(float)position.x, -(float)position.y, 0.0f));
		viewMatrix = glm::scale(viewMatrix, glm::vec3(zoom, zoom, 1.0f));
		viewMatrix = glm::rotate(viewMatrix, DegToRad(rotation), glm::vec3(0,0,1));

		PostProcessing* pp = attachedObj->GetComponent<PostProcessing>();
		if (pp == nullptr) return;
		CaptureWithPostProcessing(pp);
}

void Camera::MoveTo(Vector2<int> target, float targetTime) {
	position = target;   //No interpolation yet sincde no generator UPDATE 06/2022:: Generators??? Are you kiding
}