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

	bool m_Grounded;

public:
	Body();
	~Body();

	glm::vec3 m_Velocity;
	float m_AngularVelocity;
	glm::vec3 m_PotentialPosition;

	float m_ColliderRadius;
	bool m_BlockBlockPlacement;

	virtual void Update(float deltaTime) override;
	virtual void OnCollision(Body* body);
	virtual void OnCollision(BlockGroup* blockGroup, BlockCollisions side);
	virtual void Move();
	virtual void Draw() override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Body; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};

