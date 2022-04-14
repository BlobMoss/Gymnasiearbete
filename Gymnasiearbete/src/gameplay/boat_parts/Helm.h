#pragma once

#include "BoatPart.h"

class Helm : public BoatPart
{
protected:
	const float m_Speed = 0.3f;

	Model* m_WheelModel;
	float m_Value; // -1.0 : full left, 0.0 : no turning, 1.0 : full right

public:
	Helm();
	~Helm();

	void Update(float deltaTime) override;
	void Draw() override;

	void Interact(float deltaTime) override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Helm; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual void ForcedSetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};

