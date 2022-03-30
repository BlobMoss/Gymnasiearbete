#pragma once
#include "Sprite.h"
#include "BlockGroup.h"
#include "Raycast.h"

class Player;

class BlockCursor : public Sprite
{
public:
	bool m_Visable;

	Player* m_Player;

	RayHit m_Highlighted;
	RayHit m_Selected;

	float m_BreakTime;

public:
	BlockCursor(Player* player);
	~BlockCursor();

	void SetTransform(RayHit hit);

	void Update(float deltaTime) override;
	void Draw() override;
};

