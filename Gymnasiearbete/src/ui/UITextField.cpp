#include "UITextField.h"

#include <winuser.h>

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
		for (char KEY = 32; KEY <= 96; KEY++)
		{
			if (Input::KeyDown(KEY)) AddChar(KEY);
		}
	}

	if (Input::KeyHeld(KEY_LEFT_CONTROL) && Input::KeyDown(KEY_V))
	{
		// Delete V
		DeleteLast();

		OpenClipboard(NULL);
		std::string clipboard = getClipboardText();
		for (unsigned int i = 0; i < clipboard.length(); i++)
		{
			char c = clipboard[i];
			if (m_TextValue.length() < 32 && c >= 32 && c <= 96)
			{
				AddChar(c);
			}
		}
	}

	if (Input::KeyDown(KEY_BACKSPACE))
	{
		DeleteLast();
	}
	if (Input::KeyHeld(KEY_BACKSPACE))
	{
		if (m_DeleteTime <= 0.0f)
		{
			DeleteLast();

			m_DeleteTime = 0.05f;
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

void UITextField::AddChar(char c)
{
	SetText(m_TextValue + c);
}

void UITextField::DeleteLast()
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

std::string UITextField::getClipboardText() {
	std::string ret;
	if (::OpenClipboard(NULL)) {
		HGLOBAL hGlobal = ::GetClipboardData(CF_UNICODETEXT);
		if (hGlobal != NULL) {
			LPWSTR lpszData = (LPWSTR)::GlobalLock(hGlobal);
			if (lpszData != nullptr) {
				int size = ::WideCharToMultiByte(CP_UTF8, 0, lpszData, -1, nullptr, 0, nullptr, nullptr);
				if (size > 0) {
					ret.resize(size + 1);
					::WideCharToMultiByte(CP_UTF8, 0, lpszData, -1, &ret[0], size, nullptr, nullptr);
				}
				::GlobalUnlock(hGlobal);
			}
		}
		::CloseClipboard();
	}
	return ret;
}