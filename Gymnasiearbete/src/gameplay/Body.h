#pragma once

#include "Sprite.h"

#include "../Input.h"

class Body : public Sprite
{
protected:
	float m_Gravity;

	float m_ColliderRadius;

	glm::vec3 m_Velocity;

public:
	Body();
	~Body();

	void Update(float deltaTime) override;
	void Draw() override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Body; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};

