#include "Mast.h"

Mast::Mast()
{
	m_Model = new Model("res/models/mast.obj", "res/textures/mast.png", "res/shaders/lighting.shader");
	m_ColliderRadius = 0.35f;
}
Mast::~Mast()
{

}

void Mast::Update(float deltaTime)
{
	if (m_Parent != nullptr)
	{
		m_Parent->m_Velocity += glm::vec2(deltaTime, 0.0f);
	}

	BoatPart::Update(deltaTime);
}

void Mast::SetDescription(std::vector<uint8_t>& desc)
{
	desc >> m_WillBeRemoved >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
}
std::vector<uint8_t> Mast::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_Velocity << m_WillBeRemoved;
	return desc;
}