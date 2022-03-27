#pragma once

#include "UISprite.h"
#include "UIText.h"
#include "UIButton.h"
#include "UITextField.h"

enum class MenuState : uint32_t
{
	Main,
	InputIP,
	Connecting,
	ConnectionFailed,
};

class UIMenuManager
{
private:
	// Main menu:

	UIButton* m_StartButton;
	UIButton* m_HostButton;
	UIButton* m_ExitButton;

	// Join menu:
	
	UISprite* m_IPImage;
	UITextField* m_IPTextField;
	UIButton* m_IPAcceptButton;
	UIButton* m_IPBackButton;

	// Connecting menu:
	UISprite* m_ConnectingImage;

	// Connection failed menu
	UISprite* m_FailedImage;
	UIButton* m_FailedBackButton;

public:
	MenuState m_MenuState = MenuState::Main;

public:

	UIMenuManager();
	~UIMenuManager();

	std::string GetIPInput();

	void Update(float deltaTime);
};
