#include "Mast.h"

#include "../SpriteManager.h"

Mast::Mast()
{
	m_Model = new Model("res/models/mast.obj", "res/textures/mast.png", "res/shaders/detailed.shader");
	m_SailModel = new Model("res/models/sail.obj", "res/textures/sail.png", "res/shaders/detailed.shader");

	m_Length = 0.0f;
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
			glm::vec2 acceleration = glm::vec2(glm::sin(m_Rotation.y), glm::cos(m_Rotation.y)) * m_Length * m_Speed * 25.0f / (25.0f + m_Parent->m_Mass);

			m_Parent->m_Velocity += acceleration * deltaTime;
			m_Parent->m_AngularVelocity += -CrossProduct(glm::vec2(m_Position.x, m_Position.z) - glm::vec2(m_Parent->m_Position.x, m_Parent->m_Position.z), acceleration * 0.01f * deltaTime);
		}
	}

	BoatPart::Update(deltaTime);
}

void Mast::Draw()
{
	m_SailModel->Draw(m_Position + glm::vec3(0.0f, 7.0f, 0.0f), m_Rotation, glm::vec3(m_Scale.x, m_Scale.y * m_Length + 0.05f, m_Scale.z), m_Highlighted);

	BoatPart::Draw();
}

void Mast::Interact(float deltaTime)
{
	if (Input::KeyHeld(KEY_W))
	{
		m_Length -= deltaTime * 0.5f;
	}
	if (Input::KeyHeld(KEY_S))
	{
		m_Length += deltaTime * 0.5f;
	}
	m_Length = std::max(std::min(m_Length, 1.0f), 0.0f);

	SpriteManager::ForceUpdate(m_Id);
}

void Mast::SetDescription(std::vector<uint8_t>& desc)
{
	float junk;
	desc >> m_WillBeRemoved >> junk >> m_Occupied >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
}
void Mast::ForcedSetDescription(std::vector<uint8_t>& desc)
{
	std::vector<uint8_t> oldDesc = GetDescription();
	float newLength;
	bool newOcc;

	desc >> m_WillBeRemoved >> newLength >> newOcc >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
	oldDesc >> m_WillBeRemoved >> m_Length >> m_Occupied >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;

	m_Length = newLength;
	m_Occupied = newOcc;
}
std::vector<uint8_t> Mast::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_Velocity << m_Occupied << m_Length << m_WillBeRemoved;
	return desc;
}