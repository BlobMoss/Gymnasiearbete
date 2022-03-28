#pragma once

#include "../Body.h"
#include "../BlockGroup.h"

class BoatPart : public Body
{
protected:
	BlockGroup* m_Parent;

public:
	bool m_Interacting;

public:
	BoatPart();
	~BoatPart();

	virtual void OnCollision(BlockGroup* blockGroup, BlockCollisions side) override;

	void Update(float deltaTime) override;
};

