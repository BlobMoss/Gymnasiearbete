#include "Mast.h"

Mast::Mast()
{
	m_Model = new Model("res/models/mast.obj", "res/textures/mast.png", "res/shaders/detailed.shader");
	m_SailModel = new Model("res/models/sail.obj", "res/textures/sail.png", "res/shaders/detailed.shader");

	m_Length = 1.0f;
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
			glm::vec2 acceleration = glm::vec2(glm::sin(m_Rotation.y), glm::cos(m_Rotation.y)) * m_Length * m_Speed / (10.0f + m_Parent->m_Mass) * 10.0f;

			m_Parent->m_Velocity += acceleration * deltaTime;
			m_Parent->m_AngularVelocity += -CrossProduct(glm::vec2(m_Position.x, m_Position.z) - glm::vec2(m_Parent->m_Position.x, m_Parent->m_Position.z), acceleration * 0.01f * deltaTime);
		}
	}
	Interact(deltaTime);
	BoatPart::Update(deltaTime);
}

void Mast::Draw()
{
	m_SailModel->Draw(m_Position + glm::vec3(0.0f, 7.0f, 0.0f), m_Rotation, glm::vec3(m_Scale.x, m_Scale.y * m_Length + 0.05f, m_Scale.z));

	BoatPart::Draw();
}

void Mast::Interact(float deltaTime)
{
	if (Input::KeyHeld(KEY_W))
	{
		m_Length -= deltaTime;
	}
	if (Input::KeyHeld(KEY_S))
	{
		m_Length += deltaTime;
	}
	m_Length = std::max(std::min(m_Length, 1.0f), 0.0f);
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