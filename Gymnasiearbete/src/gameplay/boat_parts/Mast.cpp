#include "Mast.h"

Mast::Mast()
{
	m_Model = new Model("res/models/mast.obj", "res/textures/mast.png", "res/shaders/detailed.shader");

	m_ColliderRadius = 0.35f;
}
Mast::~Mast()
{

}

float CrossProduct(const glm::vec2& a, const glm::vec2& b)
{
	return a.x * b.y - a.y * b.x;
}

void Mast::Update(float deltaTime)
{
	if (m_Parent != nullptr)
	{
		if (!m_Parent->m_Static)
		{
			glm::vec2 acceleration(0.0f, 1.0f);
			acceleration = glm::vec2(
				acceleration.x * glm::cos(-m_Rotation.y) - acceleration.y * glm::sin(-m_Rotation.y),
				acceleration.x * glm::sin(-m_Rotation.y) + acceleration.y * glm::cos(-m_Rotation.y)
			);

			m_Parent->m_Velocity += acceleration * deltaTime;
			m_Parent->m_AngularVelocity += -CrossProduct(glm::vec2(m_Position.x, m_Position.z) - glm::vec2(m_Parent->m_Position.x, m_Parent->m_Position.z), acceleration * 0.01f * deltaTime);
		}
	}

	BoatPart::Update(deltaTime);
}

void Mast::SetDescription(std::vector<uint8_t>& desc)
{
	desc >> m_WillBeRemoved >> m_Length >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
}
std::vector<uint8_t> Mast::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_Velocity << m_Length << m_WillBeRemoved;
	return desc;
}