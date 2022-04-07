#pragma once

#include "Body.h"

#include "BlockData.h"

class DroppedItem : public Body
{
private:
	unsigned char m_Type;

	float m_Time;
	float m_DecayTime;

	Mesh GenerateMesh(unsigned char type = 1);
	Mesh GenerateBlockMesh();

public:
	DroppedItem(unsigned char type = 1);
	~DroppedItem();

	void Update(float deltaTime) override;
	void Draw() override;

	virtual void OnCollision(BlockGroup* blockGroup, BlockCollisions side) override;

	virtual SpriteTypes GetType() override { return SpriteTypes::DroppedItem; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};
