#pragma once

#include "Renderer.h"

#include "Sprite.h"
#include "Input.h"

class Player : public Sprite
{
private:
	float m_MoveSpeed;

public:
	Player();
	~Player();

	void Update(float deltaTime) override;
	void Draw() override;
};
