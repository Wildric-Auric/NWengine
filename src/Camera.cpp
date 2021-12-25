#include "Camera.h"

Camera::Camera(float minX, float maxX, float minY, float maxY) {
	projectionMatrix = glm::ortho(minX, maxX, minY, maxY);
	position = Vector2<int>(0, 0);
};

void Camera::Update() {
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-(float)position.x, -(float)position.y, 0.0f));
	viewMatrix = glm::scale(viewMatrix, glm::vec3(zoom, zoom, 0.0f)); //Called every frame, not optimized i'm just testing
}

void Camera::MoveTo(Vector2<int> target, float targetTime) {
	position = target;
}