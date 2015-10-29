#include "stdafx.h"
#include "Player.h"

Player::Player() {
	_horizontalAngle = 0.0f;
	_verticalAngle = 0.0f;
	_position = glm::vec3(0, 1.85, -5);
}

void Player::MoveForward(float delta) {
	_position += GetDirection() * delta;
}

void Player::MoveBackward(float delta) {
	_position -= GetDirection() * delta;
}

void Player::MoveUp(float delta) {
	_position += glm::vec3(0, 1, 0) * delta;
}

void Player::MoveDown(float delta) {
	_position -= glm::vec3(0, 1, 0) * delta;
}

void Player::MoveRight(float delta) {
	glm::vec3 right = glm::vec3(
		sin(_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(_horizontalAngle - 3.14f / 2.0f)
	);

	_position += right * delta;
}

void Player::MoveLeft(float delta) {
	glm::vec3 right = glm::vec3(
		sin(_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(_horizontalAngle - 3.14f / 2.0f)
	);

	_position -= right * delta;
}

void Player::UpdateAngles(float deltaHorizontal, float deltaVertical) {
	_horizontalAngle += deltaHorizontal;
	_verticalAngle += deltaVertical;

	if (_verticalAngle > M_PI / 2)
		_verticalAngle = M_PI / 2;

	if (_verticalAngle < -M_PI / 2)
		_verticalAngle = -M_PI / 2;
}

glm::vec3 Player::GetDirection() const {
	return glm::vec3(
		cos(_verticalAngle) * sin(_horizontalAngle),
		sin(_verticalAngle),
		cos(_verticalAngle) * cos(_horizontalAngle)
	);
}

glm::vec3 Player::GetPosition() const {
	return _position;
}

glm::mat4 Player::GetViewMatrix() const {
	return glm::lookAt(
		_position,
		_position + GetDirection(),
		glm::vec3(0, 1, 0)
	);
}
