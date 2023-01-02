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

using namespace std::string_literals;
void Camera::Gui() {
	static std::string array = (ActiveCamera->attachedObj->name + "\0"s);
	static int arraySize = 1;
	static int camIndex = 0;

	array = "";
	uint16 count = 0;
	//arraySize = cam->componentList.size();
	//for (auto it = cam->componentList.begin(); it != cam->componentList.end(); it++) {
	//	array += it->first->name + "\0"s;
	//	if (count == camIndex) cam->ActiveCamera = &it->second;
	//	count += 1;
	//}

	if (ImGui::Combo("Active Camera", &camIndex, array.c_str(), arraySize)) {
		//TODO::Only update array if user interacts with combo
	};

	NWGui::DragValue<int>("Camera Position", &(position.x), ImGuiDataType_S32, 2);
	if (NWGui::DragValue<int>("Camera Size", &(size.x), ImGuiDataType_S32, 2)) {
		ChangeOrtho(size.x, size.y);
		fbo = FrameBuffer(size.x, size.y); //TODO:: GPU Memory leak
		viewPortSize.x = size.x;
		viewPortSize.y = size.y;
	}

	//if (ImGui::DragInt2("Viewport", &(cam->viewPortSize.x))) {
	//	cam->fbo = FrameBuffer(cam->viewPortSize.x, cam->viewPortSize.y); //TODO:: NOT DO THIS HERE; just testing
	//};
}