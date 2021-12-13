#include "Body.h"

Body::Body()
	: m_Gravity(30.0f)
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