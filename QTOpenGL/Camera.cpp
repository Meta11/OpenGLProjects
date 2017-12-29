#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\transform.hpp>

const float Camera::viewSpeed = 0.007f;
const float Camera::movementSpeed = 0.1f;

Camera::Camera() : viewDirection(0.0f, 0.0f, -1.0f), UP(0.0f, 1.0f, 0.0f) {
	toRotateAround = glm::cross(viewDirection, UP);
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition) {

	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	glm::mat4 rotator = glm::rotate(viewSpeed*mouseDelta.x, UP) *
						glm::rotate(viewSpeed*mouseDelta.y, toRotateAround);

	if (glm::length(mouseDelta) < 70.0f)
		viewDirection = glm::mat3(rotator)*viewDirection;

	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix() const {
	return glm::lookAt(position, position+viewDirection, UP);
}

void Camera::moveForward() {
	position += viewDirection * movementSpeed;
}
void Camera::moveLeft() {
	position -= toRotateAround * movementSpeed;
}
void Camera::moveBackwards() {
	position -= viewDirection *movementSpeed;
}
void Camera::moveRight() {
	position += toRotateAround * movementSpeed;
}
void Camera::moveUp() {
	position += UP * movementSpeed;
}
void Camera::moveDown() {
	position -= UP * movementSpeed;
}