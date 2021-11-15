#include "Input.h"

int Input::m_DownKeys[350];
int Input::m_HeldKeys[350];
int Input::m_UpKeys[350];

Input::Input(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyCallback);
}
Input::~Input()
{

}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		m_DownKeys[key] = true;
		m_HeldKeys[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		m_UpKeys[key] = true;
		m_HeldKeys[key] = false;
	}
}

bool Input::KeyDown(int key)
{
	return m_DownKeys[key];
}
bool Input::KeyHeld(int key)
{
	return m_HeldKeys[key];
}
bool Input::KeyUp(int key)
{
	return m_UpKeys[key];
}

void Input::Update(float deltaTime)
{
	memset(m_DownKeys, false, sizeof(m_DownKeys));
	memset(m_UpKeys, false, sizeof(m_UpKeys));
}