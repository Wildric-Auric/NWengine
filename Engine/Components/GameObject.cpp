#include <GL/glew.h>
#include <glfw3.h>
#include"GameObject.h"
#include "Inputs.h"
#include "Maths.h"
#include "Camera.h"
#include "RessourcesLoader.h"
#include "Components.h"
#include "Scene.h"
//Notice that uniforms sent to shaders here are shared by all of them, I should find a way to make this better
void GameObject::Draw(int8 textureSlot) {
	Sprite* sprite = GetComponent<Sprite>();

	if (sprite == nullptr) return;		//TODO::Improve this test, not testing GameObjects with no sprite
	Transform* transform = GetComponent<Transform>();
	if (transform == nullptr) transform = this->AddComponent<Transform>();

	fVec2 position = transform->position;
	fVec2 scale = transform->scale;
	

	sprite->container.position = position;
	sprite->shader->Use();
	sprite->shader->SetUniform1f("uTime", Globals::uTime);
	sprite->shader->SetVector2("uMouse", (float)Inputs::mousePosX, (float)(Inputs::mousePosY));
	sprite->shader->SetUniform1i("uTex0", textureSlot);
	sprite->shader->SetVector2("uResolution", (float) Context::NATIVE_WIDTH, (float) Context::NATIVE_HEIGHT);
	glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((double)position.x, (double)position.y, sprite->zbuffer));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
	sprite->shader->SetMat4x4("uMvp", &(Camera::ActiveCamera->projectionMatrix * Camera::ActiveCamera->viewMatrix * model)[0][0]);

	sprite->texture->Bind(textureSlot);
	sprite->container.Draw();
};

void GameObject::BasicDraw(int8 textureSlot) {

	Sprite* sprite = GetComponent<Sprite>();
	if (sprite == nullptr) return;		//TODO::Improve this test, not testing GameObjects with no sprite
	Transform* transform = GetComponent<Transform>();
	if (transform == nullptr) transform = this->AddComponent<Transform>();

	fVec2 position = transform->position;
	fVec2 scale = transform->scale;

	sprite->container.position = position;
	glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)position.x, (float)position.y, 0.0f));
	model = glm::scale(model, glm::vec3(sign(scale.x), sign(scale.y), 1.0f));   //Flip image if should flip  
	sprite->shader->SetMat4x4("uMvp", &(Camera::ActiveCamera->projectionMatrix * Camera::ActiveCamera->viewMatrix * model)[0][0]);
	sprite->container.Draw();
}
int GameObject::numberOfGameObjects = 0;

void GameObject::Rename(std::string newName) {
	uint16 n = 0;
	name = newName;
	while (1) {
		bool br = 1;
		for (auto it = Scene::currentScene->sceneObjs.begin(); it != Scene::currentScene->sceneObjs.end(); it++) {
			if (it->name == name && &(*it) != this) {
				n += 1;
				br = 0;
				name = newName + std::to_string(n);
			}
		}
		if (br) break;
	}
	
	if (n == 0) name = newName;
	else name = newName + std::to_string(n);

}

void GameObject::AddToRender() {
	isRendered = 1;
	Scene::currentScene->drawList.push_back(this);
}

void GameObject::StopRendering() {
	isRendered = 0;
}

GameObject::GameObject() {
	numberOfGameObjects += 1;
};

GameObject::~GameObject() {};
//std::map< GameObject*, GameComponent > GameComponent::componentList;

Collider::Collider(GameObject* attachedObj, Vector2<int> offset, Vector2<int>* newSize) {
	this->offset = offset;
	this->attachedObj = attachedObj;
	if (newSize != nullptr) size = newSize;
	Transform* transform = attachedObj->GetComponent<Transform>();
	position = &transform->position;
};

void Collider::Resize(Vector2<int> newSize) {
	manualSize = newSize; 
	size = &manualSize;
};
Vector2<int> Collider::GetPosition() {
	if (this != nullptr)
		return (*position) + offset;
	return Vector2<int>(0, 0);
};
Vector2<int> Collider::GetSize() {
	if (this != nullptr) {
		Transform* transform = attachedObj->GetComponent<Transform>();
		fVec2 a =  transform->scale;
		iVec2 b = attachedObj->GetComponent<Sprite>()->texture->size;
		return iVec2(abs(a.x*b.x), abs(a.y*b.y));
	}
	return iVec2(0, 0);
}

