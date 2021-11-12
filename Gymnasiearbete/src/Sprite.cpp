#include "Sprite.h"

Sprite::Sprite()
	: m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Model(NULL)
{
	
}
Sprite::Sprite(Model* model)
	: m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Model(model)
{

}
Sprite::~Sprite()
{
	if (m_Model == NULL) return;

	delete m_Model;
}

void Sprite::Update(float deltaTime)
{
	m_Rotation.x = 0.5f;
	m_Rotation += glm::vec3(0.0f, deltaTime * 0.25f, 0.0f);
}
void Sprite::Draw(Renderer renderer)
{
	if (m_Model == NULL) return;

	m_Model->Draw(renderer, m_Position, m_Rotation);
}

void Sprite::SetPosition(glm::vec3 p)
{
    m_Position = p;
}
glm::vec3 Sprite::GetPosition()
{
    return m_Position;
}
void Sprite::SetRotation(glm::vec3 r)
{
    m_Rotation = r;
}
glm::vec3 Sprite::GetRotation()
{
    return m_Rotation;
}