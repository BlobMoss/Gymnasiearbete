#include "Body.h"

#include "BlockGroup.h"

Body::Body()
	: m_Gravity(30.0f), m_ColliderRadius(0.5f), m_BlockBlockPlacement(true)
{

}
Body::~Body()
{

}

void Body::Update(float deltaTime)
{
	m_Velocity.y += -m_Gravity * deltaTime;
	if (m_Position.y <= -1.5f || m_Grounded) m_Velocity.y = glm::max(m_Velocity.y, -1.5f);

	m_Grounded = false;

	m_PotentialPosition = m_Position + m_Velocity * deltaTime;
	m_PotentialPosition.y = glm::max(m_PotentialPosition.y, -1.5f);

	Sprite::Update(deltaTime);
}

void Body::OnCollision(Body* body)
{

}
void Body::OnCollision(BlockGroup* blockGroup, BlockCollisions side)
{
	if (side == BlockCollisions::Floor)
	{
		m_Grounded = true;
	}
}

void Body::Move()
{
	m_Position = m_PotentialPosition;
}

void Body::Draw()
{
	Sprite::Draw();
}

void Body::SetDescription(std::vector<uint8_t>& desc)
{
	desc >> m_WillBeRemoved >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
}
std::vector<uint8_t> Body::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_Velocity << m_WillBeRemoved;
	return desc;
}