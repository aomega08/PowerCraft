#pragma once

#include "stdafx.h"

#define M_PI 3.1415926f

class Player {
public:
	Player();

	void MoveForward(float delta);
	void MoveBackward(float delta);
	void MoveUp(float delta);
	void MoveDown(float delta);
	void MoveRight(float delta);
	void MoveLeft(float delta);

	void UpdateAngles(float deltaHorizontal, float deltaVertical);

	glm::vec3 GetDirection() const;
	glm::vec3 GetPosition() const;

	glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 _position;
	float _horizontalAngle, _verticalAngle;
};
