#pragma once

#include "Sprite.h"

#include "../Renderer.h"
#include "../Input.h"

class Body : public Sprite
{
protected:
	float m_Gravity;

	glm::vec3 m_Velocity;

public:
	Body();
	~Body();

	void Update(float deltaTime) override;
	void Draw() override;
};

