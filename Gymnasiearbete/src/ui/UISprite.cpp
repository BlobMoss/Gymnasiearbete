#include "UISprite.h"

UISprite::UISprite()
	: m_Position(glm::uvec2(0)), m_Image(nullptr),
	m_WillBeRemoved(false)
{

}
UISprite::UISprite(Image* image)
	: m_Position(glm::uvec2(0)), m_Image(image),
	m_WillBeRemoved(false)
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

	m_Image->Draw(m_Position);
}

void UISprite::SetPosition(glm::uvec2 p)
{
	m_Position = p;
}
glm::uvec2 UISprite::GetPosition() const
{
	return m_Position;
}

void UISprite::Remove()
{
	m_WillBeRemoved = true;
}
bool UISprite::WillBeRemoved()
{
	return m_WillBeRemoved;
}
