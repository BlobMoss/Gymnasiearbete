#pragma once

#include "Sprite.h"
#include "BlockGroup.h"

#include "../Input.h"

class Body : public Sprite
{
protected:
	float m_Gravity;

public:
	Body();
	~Body();

	glm::vec3 m_Velocity;

	float m_ColliderRadius;

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
	virtual void Move(float deltaTime);
	virtual void OnCollision(Body* body);
	virtual void OnCollision(BlockGroup* blockGroup);

	virtual SpriteTypes GetType() override { return SpriteTypes::Body; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};

