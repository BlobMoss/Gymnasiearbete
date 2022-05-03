#pragma once

#include "Body.h"

class Bullet : public Body
{
protected:
	const float m_Speed = 15.0f;

	float m_LastDelta;

	const float m_FlyDuration = 1.3f;
	float m_FlyTime;

public:
	Bullet();
	~Bullet();

	virtual void OnCollision(Body* body) override;
	virtual void OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side) override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Bullet; }

	void Update(float deltaTime) override;
	void Move() override;
};
