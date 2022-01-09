#pragma once

#include "Sprite.h"
#include "BlockGroup.h"

#include "../Input.h"

enum class BlockCollisions : unsigned char
{
	Floor,
	Side,
	Wall,
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
	glm::vec3 m_PotentialPosition;

	float m_ColliderRadius;

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
	virtual void Move();
	virtual void OnCollision(Body* body);
	virtual void OnCollision(BlockGroup* blockGroup, BlockCollisions side);

	virtual SpriteTypes GetType() override { return SpriteTypes::Body; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};

