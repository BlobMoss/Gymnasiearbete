#include "Body.h"

Body::Body()
	: m_Gravity(30.0f), m_ColliderRadius(0.6f)
{

}
Body::~Body()
{

}

void Body::Update(float deltaTime)
{
	m_Velocity.y += -m_Gravity * deltaTime;
	if (m_Position.y == 0.0f) m_Velocity.y = 0.0f;

	Sprite::Update(deltaTime);
}

void Body::Draw()
{
	Sprite::Draw();
}

void Body::Move(float deltaTime)
{
	m_Position += m_Velocity * deltaTime;
	m_Position.y = glm::max(m_Position.y, 0.0f);
}
void Body::OnCollision(Body* body)
{

}
void Body::OnCollision(BlockGroup* blockGroup)
{

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