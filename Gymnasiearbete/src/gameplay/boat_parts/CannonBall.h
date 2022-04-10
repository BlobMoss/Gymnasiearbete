#pragma once

#include "../Body.h"

struct fireParticle {
	glm::vec3 position;
	float scale;
};

class CannonBall : public Body
{
protected:
	Model* m_ParticleModel;

	const float m_Speed = 15.0f;

	const float m_FlyDuration = 2.0f;
	float m_FlyTime;

	const float m_ParticleDelay = 0.05f;
	float m_ParticleTime;

	std::vector<fireParticle> m_Particles;

public:
	glm::vec3 m_StartVelocity;

public:
	CannonBall();
	~CannonBall();

	virtual SpriteTypes GetType() override { return SpriteTypes::CannonBall; }

	void Update(float deltaTime) override;
	void Draw() override;
};
