#pragma once

#include "BoatPart.h"

class Mast : public BoatPart
{
protected:
	const float m_Speed = 3.0f;

	Model* m_SailModel;
	float m_Length;

public:
	Mast();
	~Mast();

	void Update(float deltaTime) override;
	void Draw() override;

	void Interact(float deltaTime) override;

	void Remove() override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Mast; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual void ForcedSetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};

