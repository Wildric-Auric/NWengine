#include <GL/glew.h>
#include <glfw3.h>
#include"GameObject.h"
#include "Inputs.h"
#include "Maths.h"
#include "RessourcesLoader.h"
//Notice that uniforms sent to shaders here are shared by all of them, I should find a way to make this better
void GameObject::Draw(int8 textureSlot) {
	container.position = position;
	glUseProgram(shader->shaderProgram);
	shader->SetUniform1f("uTime", uTime);
	shader->SetVector2("uMouse", (float)mousePosX, (float)(mousePosY));
	shader->SetUniform1i("uTex0", textureSlot);

	shader->SetVector2("uResolution", static_cast<float>(ORIGINAL_WIDTH), static_cast<float>(ORIGINAL_HEIGHT));
	glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)position.x, (float)position.y, 0.0f));
	model = glm::scale(model, glm::vec3(sign(scale.x), sign(scale.y), 1.0f));   //Flip image if should flip  
	shader->SetMat4x4("uMvp", &(projectionMatrix * viewMatrix * model)[0][0]);

	texture->Bind(textureSlot);
	container.Draw();
};

void GameObject::BasicDraw(int8 textureSlot) {
	container.position = position;
	shader->SetVector2("uResolution", static_cast<float>(ORIGINAL_WIDTH), static_cast<float>(ORIGINAL_HEIGHT));
	glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)position.x, (float)position.y, 0.0f));
	model = glm::scale(model, glm::vec3(sign(scale.x), sign(scale.y), 1.0f));   //Flip image if should flip  
	shader->SetMat4x4("uMvp", &(projectionMatrix * viewMatrix * model)[0][0]);
	container.Draw();
}
int GameObject::numberOfGameObjects = 0;

GameObject::GameObject(Texture* texture, Vector2<int> position, Vector2<float> scale, std::string ref, Shader* shader, bool usingImageSize, Vector2<int> size) 
{

	this->position = position;
	this->scale = scale;
	this->texture = texture;
	this->name = ref;
	//this->id = numberOfGameObjects;
	//TODO::Find a way to implement universal unique id


	numberOfGameObjects += 1;
	if (shader == nullptr) this->shader = &shaders["shader_default"];
	else this->shader = shader;
	if (texture != nullptr && usingImageSize) {
		this->size = Vector2<int>(texture->size.x * abs(scale.x), texture->size.y * abs(scale.y));
	}
	else {
		this->size = Vector2<int>(size.x * abs(scale.x), size.y * abs(scale.y));
	}

	container = Quad(position, this->size.x, this->size.y);
};

Collider::Collider(GameObjectClone* attachedObj, Vector2<int> offset, Vector2<int>* newSize) {
	this->offset = offset;
	this->attachedObj = attachedObj;
	if (newSize != nullptr) size = newSize;
	else size = &attachedObj->originalGameObject->size;
	position = &attachedObj->position;
};

std::map<GameObjectClone*, Collider> Collider::componentList;
std::map<GameObjectClone*, Script> Script::componentList;


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
		fVec2 a =  attachedObj->scale;
		iVec2 b = attachedObj->originalGameObject->size;   //GameObject.scale is deprecated, GameObject is depracated TODO::change it to gamecomponent sprite
		return iVec2(abs(a.x*b.x), abs(a.y*b.y));
	}
	return iVec2(0, 0);
}


GameObjectClone::GameObjectClone(GameObject* gameObject, const char* name) {
	this->name = _strdup(name);
	this->originalGameObject =  gameObject;
	this->position = originalGameObject->position; 
}

void GameObjectClone::Draw(int8 slot) {
	originalGameObject->container.position = position;
	glUseProgram(originalGameObject->shader->shaderProgram);
	glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)position.x, (float)position.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));   //Flip image if should flip  
	originalGameObject->shader->SetMat4x4("uMvp", &(projectionMatrix * viewMatrix * model)[0][0]);

	originalGameObject->shader->SetUniform1f("uTime", uTime);
	originalGameObject->shader->SetVector2("uResolution", static_cast<float>(ORIGINAL_WIDTH), static_cast<float>(ORIGINAL_HEIGHT));
	originalGameObject->shader->SetVector2("uMouse", (float)mousePosX, (float)(mousePosY));
	originalGameObject->texture->Bind(slot);
	originalGameObject->shader->SetUniform1i("uTex0", slot);
	originalGameObject->container.Draw();
}


void GameObjectClone::BasicDraw(int8 slot) {
	originalGameObject->container.position = position;
	glUseProgram(originalGameObject->shader->shaderProgram);
	glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)position.x, (float)position.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));   //Flip image if should flip  
	originalGameObject->shader->SetMat4x4("uMvp", &(projectionMatrix * viewMatrix * model)[0][0]);
	originalGameObject->shader->SetVector2("uResolution", static_cast<float>(ORIGINAL_WIDTH), static_cast<float>(ORIGINAL_HEIGHT));
	originalGameObject->container.Draw();
}


Script::Script(GameObjectClone* attachedObj) {
	this->attachedObj = attachedObj;
}


