#pragma once

#include "../Body.h"
#include "../BlockGroup.h"

class BoatPart : public Body
{
protected:
	BlockGroup* m_Parent;

public:
	BoatPart();
	~BoatPart();

	virtual void OnCollision(BlockGroup* blockGroup, BlockCollisions side) override;

	virtual void Update(float deltaTime) override;

	virtual void Interact(float deltaTime);
};

