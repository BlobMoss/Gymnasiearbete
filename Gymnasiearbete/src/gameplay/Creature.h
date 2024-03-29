#pragma once

#include "Body.h"

class Creature : public Body
{
protected:
	float m_FlashTime;
	const float m_FlashDuration = 0.25f;
	glm::vec4 m_FlashColor;

public:
	float m_MaxHealth;
	float m_Health;

	bool m_Dead;

public:
	Creature();
	~Creature();

	virtual void Update(float deltaTime) override;

	virtual void GetHit(float damage, glm::vec3 knockBack);
	virtual void Die();
};

