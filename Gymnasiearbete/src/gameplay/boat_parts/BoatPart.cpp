#include "BoatPart.h"
BoatPart::BoatPart()
{
	m_Static = true;

	m_FlashColor = glm::vec4(1.5f, 1.5f, 1.5f, 1.0f);
}
BoatPart::~BoatPart()
{

}

void BoatPart::OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side)
{
	if (side == BlockCollisions::Floor)
	{
		m_Parent = blockGroup;
	}

	Body::OnCollision(blockGroup, blockPos, side);
}

void BoatPart::Update(float deltaTime)
{
	if (m_Position.y < -0.01f) Die();

	m_Highlighted = false;

	Creature::Update(deltaTime);
}

void BoatPart::Interact(float deltaTime)
{

}