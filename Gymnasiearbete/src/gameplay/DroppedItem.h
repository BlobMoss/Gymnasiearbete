#pragma once

#include "Body.h"

#include "BlockData.h"

class DroppedItem : public Body
{
protected:
	float m_Time;
	float m_DecayTime;

	Mesh GenerateMesh(unsigned char type = 1);
	Mesh GenerateBlockMesh();

public:
	unsigned char m_Type;
	unsigned char m_Count;

public:
	DroppedItem(unsigned char type = 1);
	~DroppedItem();

	void Update(float deltaTime) override;
	void Draw() override;

	virtual void OnCollision(Body* body) override;
	virtual void OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side) override;

	virtual SpriteTypes GetType() override { return SpriteTypes::DroppedItem; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};
