#pragma once

#include "Body.h"

#include "../Renderer.h"
#include "../Input.h"

class Player : public Body
{
private:
	float m_MoveSpeed;

public:
	Player();
	~Player();

	void Update(float deltaTime) override;
	void Draw() override;
};
