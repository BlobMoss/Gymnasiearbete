#include "UISprite.h"

UISprite::UISprite()
	: m_Position(glm::ivec2(0)), m_Scale(glm::vec2(1.0f)), m_Color(glm::vec4(1.0f)), m_SortingOrder(0.5f), m_Image(nullptr),
	m_Active(true), m_WillBeRemoved(false)
{

}
UISprite::UISprite(Image* image)
	: m_Position(glm::ivec2(0)), m_Scale(glm::vec2(1.0f)), m_Color(glm::vec4(1.0f)), m_SortingOrder(0.5f), m_Image(image),
	m_Active(true), m_WillBeRemoved(false)
{

}
UISprite::~UISprite()
{
	// Also delete image if sprite has one
	if (m_Image == nullptr) return;

	delete m_Image;
}

void UISprite::Update(float deltaTime)
{

}
void UISprite::Draw()
{
	if (m_Image == nullptr) return;

	m_Image->Draw(m_Position, m_Scale, m_Color, m_SortingOrder);
}

void UISprite::Remove()
{
	m_WillBeRemoved = true;
}
bool UISprite::WillBeRemoved()
{
	return m_WillBeRemoved;
}
