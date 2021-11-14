#pragma once

#include "Renderer.h"

class Sprite;

class Camera
{
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;

	glm::vec2 m_PositionOffset;
	float m_ViewAngle;

	Sprite* m_FollowTarget;

public:
	Camera();
	~Camera();

	void Update(float deltaTime);

	void SetFollowTarget(Sprite& target);

	void SetPosition(glm::vec3 p);
	glm::vec3 GetPosition() const;
	void SetRotation(glm::vec3 r);
	glm::vec3 GetRotation() const;
};
