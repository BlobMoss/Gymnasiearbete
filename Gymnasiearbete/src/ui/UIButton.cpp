#include "UIButton.h"
#include "../Input.h"

UIButton::UIButton(Image* image) 
{
	m_Image = image;
}
UIButton::UIButton()
{
	m_Image = nullptr;
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
		mousePos.x < m_Position.x + m_Image->GetWidth() &&
		mousePos.y > m_Position.y && 
		mousePos.y < m_Position.y + m_Image->GetHeight();
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