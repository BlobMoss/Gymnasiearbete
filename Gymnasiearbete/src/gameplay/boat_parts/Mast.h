#pragma once

#include "BoatPart.h"

class Mast : public BoatPart
{
protected:
	float m_Length;

public:

	Mast();
	~Mast();

	void Update(float deltaTime) override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Mast; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};

