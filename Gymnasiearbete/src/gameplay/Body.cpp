#include "Body.h"

Body::Body()
	: m_Gravity(30.0f), m_ColliderRadius(1.0f)
{

}
Body::~Body()
{

}

void Body::Update(float deltaTime)
{
	m_Velocity.y += -m_Gravity * deltaTime;
	m_Position += m_Velocity * deltaTime;
	m_Position.y = glm::max(m_Position.y, 0.0f);

	Sprite::Update(deltaTime);
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