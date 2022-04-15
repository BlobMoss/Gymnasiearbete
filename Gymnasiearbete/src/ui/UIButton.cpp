#include "UIButton.h"
#include "../Input.h"

UIButton::UIButton(Image* image) 
{
	m_Image = image;
	m_ButtonWidth = image->GetWidth();
	m_ButtonHeight = image->GetHeight();
}
UIButton::UIButton(unsigned int width, unsigned int height)
{
	m_Image = nullptr;
	m_ButtonWidth = width;
	m_ButtonHeight = height;
}

UIButton::~UIButton()
{

}

bool UIButton::Hover()
{
	glm::vec2 mousePos = Input::MousePosition();
	mousePos /= Renderer::pixelSize;
	mousePos.y = referenceHeight - mousePos.y;

	return mousePos.x > m_Position.x &&
		mousePos.x < m_Position.x + m_ButtonWidth &&
		mousePos.y > m_Position.y && 
		mousePos.y < m_Position.y + m_ButtonHeight;
}
bool UIButton::Down()
{
	if (Input::MouseButtonDown(0) && m_Active)
	{
		return Hover();
	}
	return false;
}
bool UIButton::Held()
{
	if (Input::MouseButtonHeld(0) && m_Active)
	{
		return Hover();
	}
	return false;
}
bool UIButton::Up()
{
	if (Input::MouseButtonUp(0) && m_Active)
	{
		return Hover();
	}
	return false;
}