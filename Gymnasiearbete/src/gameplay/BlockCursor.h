#pragma once
#include "Sprite.h"
#include "BlockGroup.h"

class BlockCursor : public Sprite
{
public:
	bool m_Visable;

	BlockGroup* m_BlockGroup;
	glm::ivec3 m_LastEmpty;
	glm::ivec3 m_FirstBlock;

public:
	BlockCursor();
	~BlockCursor();

	void SetTransform(RayHit hit);

	void Update(float deltaTime) override;
	void Draw() override;
};

