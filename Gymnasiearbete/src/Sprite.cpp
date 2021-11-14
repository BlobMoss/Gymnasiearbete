#include "Sprite.h"

Sprite::Sprite()
	: m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Model(nullptr),
	m_WillBeRemoved(false)
{
	
}
Sprite::Sprite(Model* model)
	: m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Model(model),
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
void Sprite::Draw(Camera& camera)
{
	if (m_Model == nullptr) return;

	m_Model->Draw(camera, m_Position, m_Rotation, m_Scale);
}

void Sprite::SetPosition(glm::vec3 p)
{
    m_Position = p;
}
glm::vec3 Sprite::GetPosition() const
{
    return m_Position;
}
void Sprite::SetRotation(glm::vec3 r)
{
    m_Rotation = r;
}
glm::vec3 Sprite::GetRotation() const
{
    return m_Rotation;
}
void Sprite::SetScale(glm::vec3 s)
{
	m_Scale = s;
}
glm::vec3 Sprite::GetScale() const
{
	return m_Scale;
}

void Sprite::Remove()
{
	m_WillBeRemoved = true;
}
bool Sprite::WillBeRemoved()
{
	return m_WillBeRemoved;
}