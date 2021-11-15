#pragma once

#include "Renderer.h"

#include "InputDefinitions.h"

class Input
{
private:
	static int m_DownKeys[350];
	static int m_HeldKeys[350];
	static int m_UpKeys[350];

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	Input(GLFWwindow* window);
	~Input();

	static bool KeyDown(int key);
	static bool KeyHeld(int key);
	static bool KeyUp(int key);

	static void Update(float deltaTime);
};

