#include <GL/glew.h>
#include <glfw3.h>
#include"GameObject.h"
#include "Inputs.h"
#include "Maths.h"
#include "RessourcesLoader.h"
//Notice that uniforms sent to shaders here are shared by all of them, I should find a way to make this better
void GameObject::Draw(uint8_t textureSlot) {
	container.position = position;
	glUseProgram(shader->shaderProgram);
	glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)position.x, (float)position.y, 0.0f));
	model = glm::scale(model, glm::vec3(sign(scale.x), sign(scale.y), 1.0f));   //Flip image if should flip  
	shader->SetMat4x4("uMvp", &(projectionMatrix * viewMatrix * model)[0][0]);

	shader->SetUniform1f("uTime", uTime);
	shader->SetVector2("uResolution", static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
	shader->SetVector2("uMouse", (float)mousePosX, (float)(mousePosY));
	image->Bind(textureSlot);
	shader->SetUniform1i("uTex0", textureSlot);
	container.Draw();
};

int GameObject::numberOfGameObjects = 0;

GameObject::GameObject(Texture* image, Vector2<int> position, Vector2<float> scale, GameObject* ref, Shader* shader, bool usingImageSize, Vector2<int> size) 
{

	this->position = position;
	this->scale = scale;
	this->image = image;
	this->id = numberOfGameObjects;
	allObjects[id] = ref;


	numberOfGameObjects += 1;
	if (shader == nullptr) this->shader = &shaders["shader_default"];
	else this->shader = shader;
	if (image != nullptr && usingImageSize) {
		this->size = Vector2<int>(image->size.x * abs(scale.x), image->size.y * abs(scale.y));
	}
	else {
		this->size = Vector2<int>(size.x * abs(scale.x), size.y * abs(scale.y));
	}

	container = Quad(position, this->size.x, this->size.y);
};

Collider::Collider(GameObjectClone* attachedObj, Vector2<int> offset, Vector2<int>* newSize) {
	this->offset = offset;
	if (newSize != nullptr) size = newSize;
	else size = &attachedObj->originalGameObject->size;
	position = &attachedObj->position;
};
void Collider::Resize(Vector2<int> newSize) {
	manualSize = newSize;
	size = &manualSize;
};
Vector2<int> Collider::GetPosition() {
	return (*position) + offset;
};

GameObjectClone::GameObjectClone(GameObject* gameObject) {
	this->originalGameObject =  gameObject;
	this->position = originalGameObject->position; 
}

void GameObjectClone::Draw(uint8_t slot) {
	originalGameObject->container.position = position;
	glUseProgram(originalGameObject->shader->shaderProgram);
	glm::mat4x4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)position.x, (float)position.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));   //Flip image if should flip  
	originalGameObject->shader->SetMat4x4("uMvp", &(projectionMatrix * viewMatrix * model)[0][0]);

	originalGameObject->shader->SetUniform1f("uTime", uTime);
	originalGameObject->shader->SetVector2("uResolution", static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
	originalGameObject->shader->SetVector2("uMouse", (float)mousePosX, (float)(mousePosY));
	originalGameObject->image->Bind(slot);
	originalGameObject->shader->SetUniform1i("uTex0", slot);
	originalGameObject->container.Draw();
}