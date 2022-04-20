#include "Body.h"

#include "BlockGroup.h"

Body::Body()
	: m_Gravity(30.0f), m_ColliderRadius(0.5f), m_KnockBackDrag(0.75f), m_Static(false), m_BlockBlockPlacement(true)
{

}
Body::~Body()
{

}

void Body::Update(float deltaTime)
{
	m_Velocity.y += -m_Gravity * deltaTime;
	if (m_Position.y <= -1.5f || m_Grounded) m_Velocity.y = glm::max(m_Velocity.y, -0.1f);

	m_Grounded = false;

	m_PotentialPosition = m_Position + m_Velocity * deltaTime;
	if (!m_Static) m_PotentialPosition += m_KnockBackVelocity * deltaTime;
	m_PotentialPosition.y = glm::max(m_PotentialPosition.y, -1.5f);

	m_KnockBackVelocity -= m_KnockBackVelocity * m_KnockBackDrag * deltaTime;

	m_Rotation.y = glm::mod(m_Rotation.y, glm::pi<float>() * 2.0f);
	m_TargetRotation = glm::mod(m_TargetRotation, glm::pi<float>() * 2.0f);

	if (m_Turning)
	{
		float a = m_TargetRotation - m_Rotation.y;
		a = glm::mod(a + glm::pi<float>(), glm::pi<float>() * 2.0f) - glm::pi<float>();

		m_Rotation.y += a * deltaTime * 8.0f;
	}
	if (floor(m_TargetRotation * 1000.0f) == floor(m_Rotation.y * 1000.0f))
	{
		m_Turning = false;
	}

	Sprite::Update(deltaTime);
}

void Body::OnCollision(Body* body)
{

}
void Body::OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side)
{
	if (side == BlockCollisions::Floor)
	{
		m_Grounded = true;
	}
}

void Body::Move()
{
	if (m_SkipNextFalls > 0)
	{
		m_PotentialPosition.y = 0.0;
		m_SkipNextFalls--;
	}
	m_Position = m_PotentialPosition;
}

void Body::TurnSmoothly(float targetRotation)
{
	m_Turning = true;
	m_TargetRotation = targetRotation;
}

void Body::Draw()
{
	Sprite::Draw();
}

void Body::SetDescription(std::vector<uint8_t>& desc)
{
	desc >> m_WillBeRemoved >> m_KnockBackVelocity >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
}
std::vector<uint8_t> Body::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_Velocity << m_KnockBackVelocity << m_WillBeRemoved;
	return desc;
}