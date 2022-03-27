#pragma once
#include "UIText.h"

class UITextField : public UIText
{
protected:
	float m_DeleteTime;
	char m_EndChar = ' ';
	float m_FlashTime;

	void Deletelast();

public:
	std::string m_TextValue;

public:
	UITextField();
	~UITextField();

	void Update(float deltaTime) override;

	void SetText(const std::string& text) override;
};

