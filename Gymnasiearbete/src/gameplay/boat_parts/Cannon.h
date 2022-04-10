#pragma once

#include "BoatPart.h"

class Cannon : public BoatPart
{
protected:
	Model* m_FuseModel;

	const float m_LoadDelay = 5.0f;
	float m_LoadTime;

	bool m_Loaded;

public:
	Cannon();
	~Cannon();

	void Update(float deltaTime) override;
	void Draw() override;

	void Fire();

	virtual SpriteTypes GetType() override { return SpriteTypes::Cannon; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};

