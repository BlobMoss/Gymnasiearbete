#pragma once

#include "UISprite.h"

class UIButton : public UISprite
{
protected:
	int m_ButtonWidth;
	int m_ButtonHeight;

public:
	UIButton(Image* image);
	UIButton(unsigned int width, unsigned int height);
	~UIButton();

	bool Hover();
	bool Down();
	bool Held();
	bool Up();
};

