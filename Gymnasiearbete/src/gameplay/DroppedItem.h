#pragma once
#include "Body.h"

class DroppedItem : public Body
{
	DroppedItem();
	~DroppedItem();

	void Update(float deltaTime) override;
};
