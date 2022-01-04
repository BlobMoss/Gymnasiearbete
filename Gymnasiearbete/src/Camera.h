#pragma once

#include "Common.h"

class Sprite;

class Camera
{
private:
	static glm::vec2 m_PositionOffset;

	static glm::vec3 m_Position;
	static glm::vec3 m_Rotation;

	static float m_RotationSpeed;
	static float m_ViewAngle;

	static Sprite* m_FollowTarget;

public:
	static void Update(float deltaTime);

	static void SetFollowTarget(Sprite* target);
	static Sprite* GetFollowTarget();

	static void SetPosition(glm::vec3 p);
	static glm::vec3 GetPosition();
	static void SetRotation(glm::vec3 r);
	static glm::vec3 GetRotation();

	static float GetViewAngle();
};
