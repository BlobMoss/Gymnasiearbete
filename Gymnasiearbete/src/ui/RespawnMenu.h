#pragma once

#include "UISprite.h"
#include "UIButton.h"

#include "../gameplay/Player.h"

class RespawnMenu
{
private:
	UISprite* m_Fade;
	UISprite* m_YouDied;
	UIButton* m_RespawnButton;

	Player* m_Player;

	bool m_Respawning, m_Respawned;

public:
	static RespawnMenu* m_Instance;

public:
	RespawnMenu();
	~RespawnMenu();

	void Update(float deltaTime);
};

