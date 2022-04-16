#pragma once

#include "Body.h"

#include "BlockData.h"

class DroppedItem : public Body
{
protected:
	float m_Time;
	float m_DecayTime;

	Model* GenerateModel(unsigned char type);
	Mesh GenerateBlockMesh();
	Mesh GenerateQuadMesh();

public:
	unsigned char m_Type;
	unsigned char m_Count;

	bool m_CanBePickedUp;

public:
	DroppedItem(unsigned char type = 1, unsigned int count = 1);
	~DroppedItem();

	void Update(float deltaTime) override;
	void Draw() override;

	virtual void OnCollision(Body* body) override;

	virtual SpriteTypes GetType() override { return SpriteTypes::DroppedItem; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};
