#include "BoatPart.h"
BoatPart::BoatPart()
{
	m_Static = true;
}
BoatPart::~BoatPart()
{

}

void BoatPart::OnCollision(BlockGroup* blockGroup, BlockCollisions side)
{
	if (side == BlockCollisions::Floor)
	{
		m_Parent = blockGroup;
	}
}

void BoatPart::Update(float deltaTime)
{
	if (m_Position.y < -0.01f) Remove();

	Body::Update(deltaTime);
}

void BoatPart::Interact(float deltaTime)
{

}