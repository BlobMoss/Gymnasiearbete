#pragma once

#include "Renderer.h"

#include "InputDefinitions.h"

class Input
{
private:
	static int m_DownInput[350];
	static int m_HeldInput[350];
	static int m_UpInput[350];

	static GLFWgamepadstate m_GamepadState;
	static GLFWgamepadstate m_PrevGamepadState;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

	// Gamepad
	static bool ButtonDown(int button);
	static bool ButtonHeld(int button);
	static bool ButtonUp(int button);

	static glm::vec2 GamepadLeftStick();
	static glm::vec2 GamepadRightStick();
	static float GamepadLeftTrigger();
	static float GamepadRightTrigger();
};

