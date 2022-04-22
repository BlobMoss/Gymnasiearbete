#pragma once

#include "UISprite.h"

#include "../gameplay/Player.h"

class HealthBar
{
private:
	UISprite* m_BackGround;
	UISprite* m_Bar;

	Player* m_Player;

public:
	static HealthBar* m_Instance;

public:
	HealthBar();
	~HealthBar();

	void Update(float deltaTime);
};

