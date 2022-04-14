#pragma once

#include "Common.h"

class Sprite;
struct RayHit;

class Camera
{
private:
	static glm::vec2 m_PositionOffset;

	static float m_RotationSpeed;

	static Sprite* m_FollowTarget;

public: 
	static glm::vec3 m_Position;
	static glm::vec3 m_Rotation;
	static glm::vec3 m_TargetPosition;

	static float m_ViewAngle;

public:
	static void Update(float deltaTime);

	static RayHit RayFromScreen(glm::vec2 point);

	static void SetFollowTarget(Sprite* target);
	static Sprite* GetFollowTarget();
};
