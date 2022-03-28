#pragma once
#include "UIText.h"

class UITextField : public UIText
{
protected:
	float m_DeleteTime;
	float m_FlashTime;
	char m_EndChar = ' ';

	void AddChar(char c);
	void DeleteLast();

	std::string getClipboardText();

public:
	std::string m_TextValue;

public:
	UITextField();
	~UITextField();

	void Update(float deltaTime) override;

	void SetText(const std::string& text) override;
};

