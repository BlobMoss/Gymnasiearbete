#include "Helm.h"

#include "../SpriteManager.h"

Helm::Helm()
{
	m_Model = new Model("res/models/helm.obj", "res/textures/helm.png", "res/shaders/detailed.shader");
	m_WheelModel = new Model("res/models/wheel.obj", "res/textures/wheel.png", "res/shaders/detailed.shader");

	m_Value = 0.0f;
	m_ColliderRadius = 0.45f;
}
Helm::~Helm()
{

}

void Helm::Update(float deltaTime)
{
	if (m_Parent != nullptr)
	{
		if (!m_Parent->m_Static)
		{
			m_Parent->m_AngularVelocity += m_Value * deltaTime * m_Speed * 25.0f / (25.0f + m_Parent->m_Mass);
		}
	}

	BoatPart::Update(deltaTime);
}

void Helm::Draw()
{
	m_WheelModel->Draw(m_Position + glm::vec3(0.0f, 1.45f, 0.0f), m_Rotation + glm::vec3(0.0f, 0.0f, -m_Value * glm::pi<float>() * 2.0f), m_Scale, m_Highlighted);

	BoatPart::Draw();
}

void Helm::Interact(float deltaTime)
{
	if (Input::KeyHeld(KEY_D))
	{
		m_Value -= deltaTime * 0.25f;
	}
	if (Input::KeyHeld(KEY_A))
	{
		m_Value += deltaTime * 0.25f;
	}
	m_Value = std::max(std::min(m_Value, 1.0f), -1.0f);

	SpriteManager::ForceUpdate(m_Id);
}

void Helm::SetDescription(std::vector<uint8_t>& desc)
{
	float junk;
	desc >> m_WillBeRemoved >> junk >> m_Occupied >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
}
void Helm::ForcedSetDescription(std::vector<uint8_t>& desc)
{
	std::vector<uint8_t> oldDesc = GetDescription();
	float newValue;
	bool newOcc;
	
	desc >> m_WillBeRemoved >> newValue >> newOcc >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
	oldDesc >> m_WillBeRemoved >> m_Value >> m_Occupied >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;

	m_Value = newValue;
	m_Occupied = newOcc;
}
std::vector<uint8_t> Helm::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_Velocity << m_Occupied << m_Value << m_WillBeRemoved;
	return desc;
}