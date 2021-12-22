#pragma once
#include "Renderer.h"

#include "InputDefinitions.h"

class Input
{
private:
	static bool m_KeysDown[349];
	static bool m_KeysHeld[349];
	static bool m_KeysUp[349];

	static bool m_MouseButtonsDown[8];
	static bool m_MouseButtonsHeld[8];
	static bool m_MouseButtonsUp[8];

	static double m_CursorX, m_CursorY;

	static GLFWgamepadstate m_GamepadState;
	static GLFWgamepadstate m_PrevGamepadState;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

public:
	Input(GLFWwindow* window);
	~Input();

	static bool m_GamepadConnected;

	static void Update(float deltaTime);

	// General
	static float Horizontal();
	static float Vertical();

	static bool TurnCameraLeft();
	static bool TurnCameraRight();

	// Keyboard
	static bool KeyDown(int key);
	static bool KeyHeld(int key);
	static bool KeyUp(int key);

	// Mouse
	static bool MouseButtonDown(int button);
	static bool MouseButtonHeld(int button);
	static bool MouseButtonUp(int button);

	static glm::vec2 MousePosition();

	// Gamepad
	static bool ButtonDown(int button);
	static bool ButtonHeld(int button);
	static bool ButtonUp(int button);

	static glm::vec2 GamepadLeftStick();
	static glm::vec2 GamepadRightStick();
	static float GamepadLeftTrigger();
	static float GamepadRightTrigger();
};

