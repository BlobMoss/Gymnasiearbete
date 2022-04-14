#pragma once

#include "../Body.h"
#include "../BlockGroup.h"

class BoatPart : public Body
{
protected:
	BlockGroup* m_Parent;

public:
	bool m_Occupied;
	bool m_OccupiedHere;

public:
	BoatPart();
	~BoatPart();

	virtual void OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side) override;

	virtual void Update(float deltaTime) override;

	virtual void Interact(float deltaTime);
};

