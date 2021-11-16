#include "Input.h"

int Input::m_DownInput[350];
int Input::m_HeldInput[350];
int Input::m_UpInput[350];

bool Input::m_GamepadConnected;
GLFWgamepadstate Input::m_GamepadState;
GLFWgamepadstate Input::m_PrevGamepadState;

Input::Input(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyCallback);
}
Input::~Input()
{

}

void Input::Update(float deltaTime)
{
	memset(m_DownInput, false, sizeof(m_DownInput));
	memset(m_UpInput, false, sizeof(m_UpInput));

	m_GamepadConnected = glfwJoystickPresent(GLFW_JOYSTICK_1);

	if (m_GamepadConnected)
	{
		m_PrevGamepadState = m_GamepadState;
		glfwGetGamepadState(GLFW_JOYSTICK_1, &m_GamepadState);
	}
}

// General

float Input::Horizontal()
{
	float horizontal = 0.0f;
	horizontal -= KeyHeld(KEY_A);
	horizontal += KeyHeld(KEY_D);
	if (m_GamepadConnected) horizontal += GamepadLeftStick().x;
	return horizontal;
}
float Input::Vertical()
{
	float vertical = 0.0f;
	vertical -= KeyHeld(KEY_W);
	vertical += KeyHeld(KEY_S);
	if (m_GamepadConnected) vertical += GamepadLeftStick().y;
	return vertical;
}

// Keyboard

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		m_DownInput[key] = true;
		m_HeldInput[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		m_UpInput[key] = true;
		m_HeldInput[key] = false;
	}
}

bool Input::KeyDown(int key)
{
	return m_DownInput[key];
}
bool Input::KeyHeld(int key)
{
	return m_HeldInput[key];
}
bool Input::KeyUp(int key)
{
	return m_UpInput[key];
}

// Gamepad

bool Input::ButtonDown(int button)
{
	return m_GamepadState.buttons[button] && !m_PrevGamepadState.buttons[button];
}
bool Input::ButtonHeld(int button)
{
	return m_GamepadState.buttons[button];
}
bool Input::ButtonUp(int button)
{
	return !m_GamepadState.buttons[button] && m_PrevGamepadState.buttons[button];;
}

glm::vec2 Input::GamepadLeftStick()
{
	float x = m_GamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
	float y = m_GamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
	// Ignore tiny stick values
	x = glm::abs(x) > 0.1f ? x : 0.0f;
	y = glm::abs(y) > 0.1f ? y : 0.0f;
	return glm::vec2(x, y);
}
glm::vec2 Input::GamepadRightStick()
{
	float x = m_GamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
	float y = m_GamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
	// Ignore tiny stick values
	x = glm::abs(x) > 0.1f ? x : 0.0f;
	y = glm::abs(y) > 0.1f ? y : 0.0f;
	return glm::vec2(x, y);
}
float Input::GamepadLeftTrigger()
{
	return m_GamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
}
float Input::GamepadRightTrigger()
{
	return m_GamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
}