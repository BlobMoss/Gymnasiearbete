#include "Sprite.h"

#include "../Client.h"

Sprite::Sprite()
	: m_OwnedHere(true),
	m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f), m_Model(nullptr), m_Color(1.0f), m_Highlighted(false),
	m_Active(true), m_WillBeRemoved(false)
{

}
Sprite::Sprite(Model* model)
	: m_OwnedHere(true),
	m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f), m_Model(model), m_Color(1.0f), m_Highlighted(false),
	m_Active(true), m_WillBeRemoved(false)
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

	m_Model->Draw(m_Position, m_Rotation, m_Scale, m_Color, m_Highlighted);
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
	char objPath[50];
	char texturePath[50];
	char shaderPath[50];

	desc >> m_WillBeRemoved >> shaderPath >> texturePath >> objPath >> m_Active >> m_Scale >> m_Rotation >> m_Position;

	m_ObjPath = std::string(objPath);
	m_TexturePath = std::string(texturePath);
	m_ShaderPath = std::string(shaderPath);
}
void Sprite::ForcedSetDescription(std::vector<uint8_t>& desc)
{
	
}
std::vector<uint8_t> Sprite::GetDescription() const
{
	char objPath[50];
	strncpy_s(objPath, m_Model->m_ObjPath.c_str(), sizeof(objPath));
	char texturePath[50];
	strncpy_s(texturePath, m_Model->m_TexturePath.c_str(), sizeof(texturePath));
	char shaderPath[50];
	strncpy_s(shaderPath, m_Model->m_ShaderPath.c_str(), sizeof(shaderPath));

	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_Active << objPath << texturePath << shaderPath << m_WillBeRemoved;
	return desc;
}