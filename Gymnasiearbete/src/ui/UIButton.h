#pragma once

#include "UISprite.h"

class UIButton : public UISprite
{
protected:

public:
	UIButton(Image* image);
	~UIButton();

	bool Hover();
	bool Down();
	bool Held();
	bool Up();
};
