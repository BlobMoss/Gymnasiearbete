#include "Sprite.h"

#include "../Client.h"

Sprite::Sprite()
	: m_OwnedHere(true),
	m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Model(nullptr), m_Highlighted(false),
	m_WillBeRemoved(false)
{

}
Sprite::Sprite(Model* model)
	: m_OwnedHere(true),
	m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Model(model), m_Highlighted(false),
	m_WillBeRemoved(false)
{
	
}
Sprite::~Sprite()
{
	// Also delete model if sprite has one
	if (m_Model == nullptr) return;

	delete m_Model;
}

void Sprite::Update(float deltaTime)
{
	
}
void Sprite::Draw()
{
	if (m_Model == nullptr) return;

	m_Model->Draw(m_Position, m_Rotation, m_Scale, m_Highlighted);
}

void Sprite::Remove()
{
	m_WillBeRemoved = true;
}
bool Sprite::WillBeRemoved()
{
	return m_WillBeRemoved;
}

void Sprite::SetDescription(std::vector<uint8_t>& desc)
{
	desc >> m_WillBeRemoved >> m_Scale >> m_Rotation >> m_Position;
}
void Sprite::ForcedSetDescription(std::vector<uint8_t>& desc)
{
	
}
std::vector<uint8_t> Sprite::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_WillBeRemoved;
	return desc;
}