#include "UISprite.h"

UISprite::UISprite()
	: m_Position(glm::ivec2(0)), m_Image(nullptr),
	m_Active(true), m_WillBeRemoved(false), m_SortingOrder(0.5f)
{

}
UISprite::UISprite(Image* image)
	: m_Position(glm::ivec2(0)), m_Image(image),
	m_Active(true), m_WillBeRemoved(false), m_SortingOrder(0.5f)
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

	m_Image->Draw(m_Position, m_SortingOrder);
}

void UISprite::Remove()
{
	m_WillBeRemoved = true;
}
bool UISprite::WillBeRemoved()
{
	return m_WillBeRemoved;
}
