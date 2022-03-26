#pragma once

#include "UISprite.h"
#include "UIText.h"
#include "UIButton.h"

class UIMenuManager
{
private:
	// Main menu
	UIButton* startButton;
	UIButton* hostButton;
	UIButton* exitButton;

	// Join menu
	//UISprite* IpImage;
	//UITextField* IPfield;

public:

	UIMenuManager();
	~UIMenuManager();

	void Update(float deltaTime);
};

