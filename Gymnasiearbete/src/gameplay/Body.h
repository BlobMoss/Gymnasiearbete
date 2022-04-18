#pragma once

#include "Sprite.h"

#include "../Input.h"

// Forward decleration
class BlockGroup;

enum class BlockCollisions : unsigned char
{
	Floor,
	Side,
	Wall,
	BlockAbove,
};

class Body : public Sprite
{
protected:
	float m_Gravity;

	bool m_Turning;
	float m_TargetRotation;

	bool m_Grounded;

public:
	Body();
	~Body();

	glm::vec3 m_Velocity;
	glm::vec3 m_KnockBackVelocity;
	float m_AngularVelocity;
	glm::vec3 m_PotentialPosition;

	unsigned int m_SkipNextFalls;

	float m_ColliderRadius;
	bool m_Static;
	bool m_BlockBlockPlacement;

	int m_Health;

	virtual void Update(float deltaTime) override;
	virtual void OnCollision(Body* body);
	virtual void OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side);
	virtual void Move();
	virtual void TurnSmoothly(float targetRotation);
	virtual void Draw() override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Body; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};

