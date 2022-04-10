#pragma once
#include "Sprite.h"
#include "BlockGroup.h"
#include "Raycast.h"

class Player;

class BlockCursor : public Sprite
{
public:
	bool m_Visable;

	unsigned int m_Level;

	Player* m_Player;

	Model* m_CracksModel;

	RayHit m_Highlighted;
	RayHit m_Selected;

	const float m_PlaceDelay = 0.25f;
	float m_PlaceTime;
	float m_BreakTime;
	float m_MaxBreak;

private:
	Mesh GenerateCracksMesh();

public:
	BlockCursor(Player* player);
	~BlockCursor();

	void SetTransform(RayHit hit);

	void Update(float deltaTime) override;
	void Draw() override;
};

