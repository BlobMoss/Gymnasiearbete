#include "UITextField.h"
#include "../Input.h"

UITextField::UITextField()
{
	m_Image = new Image("res/images/font.png");
	SetText("");
}
UITextField::~UITextField()
{

}

void UITextField::Update(float deltaTime)
{
	if (m_TextValue.length() < 32)
	{
		if (Input::KeyDown(KEY_1)) SetText(m_TextValue + '1');
		if (Input::KeyDown(KEY_2)) SetText(m_TextValue + '2');
		if (Input::KeyDown(KEY_3)) SetText(m_TextValue + '3');
		if (Input::KeyDown(KEY_4)) SetText(m_TextValue + '4');
		if (Input::KeyDown(KEY_5)) SetText(m_TextValue + '5');
		if (Input::KeyDown(KEY_6)) SetText(m_TextValue + '6');
		if (Input::KeyDown(KEY_7)) SetText(m_TextValue + '7');
		if (Input::KeyDown(KEY_8)) SetText(m_TextValue + '8');
		if (Input::KeyDown(KEY_9)) SetText(m_TextValue + '9');
		if (Input::KeyDown(KEY_0)) SetText(m_TextValue + '0');
		if (Input::KeyDown(KEY_PERIOD) || Input::KeyDown(KEY_COMMA)) SetText(m_TextValue + '.');
	}

	if (Input::KeyDown(KEY_BACKSPACE))
	{
		Deletelast();
	}
	if (Input::KeyHeld(KEY_BACKSPACE))
	{
		if (m_DeleteTime <= 0.0f)
		{
			Deletelast();

			m_DeleteTime = 0.06f;
		}
		m_DeleteTime -= deltaTime;
	}
	else
	{
		m_DeleteTime = 0.6f;
	}

	if (m_FlashTime <= 0.0f)
	{
		if (m_EndChar == ' ') m_EndChar = '_';
		else m_EndChar = ' ';
		SetText(m_TextValue);

		m_FlashTime = 0.75f;
	}
	m_FlashTime -= deltaTime;
}

void UITextField::Deletelast()
{
	if (m_TextValue.length() > 0)
	{
		m_TextValue.pop_back();
		SetText(m_TextValue);
		m_Position.x += 4;
	}
}

void UITextField::SetText(const std::string& text)
{
	if (m_TextValue.length() < text.length()) m_Position.x -= 4;

	m_TextValue = text;
	m_Text = m_TextValue + m_EndChar;

	m_Mesh = GenerateMesh();

	m_Image->UpdateData(m_Mesh);
}