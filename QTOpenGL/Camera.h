#pragma once
#include <glm\glm.hpp>

class Camera {
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 toRotateAround;
	static const float viewSpeed;
	static const float movementSpeed;
	const glm::vec3 UP;
	glm::vec2 oldMousePosition;
public:
	Camera();
	void mouseUpdate(const glm::vec2& mousePosition);
	glm::mat4 getWorldToViewMatrix() const;
	void moveForward();
	void moveLeft();
	void moveBackwards();
	void moveRight();
	void moveUp();
	void moveDown();
};

