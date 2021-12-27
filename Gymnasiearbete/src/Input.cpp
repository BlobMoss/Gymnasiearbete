#include "Input.h"

bool Input::m_KeysDown[349];
bool Input::m_KeysHeld[349];
bool Input::m_KeysUp[349];

bool Input::m_MouseButtonsDown[8];
bool Input::m_MouseButtonsHeld[8];
bool Input::m_MouseButtonsUp[8];

double Input::m_CursorX, Input::m_CursorY;

bool Input::m_GamepadConnected;
GLFWgamepadstate Input::m_GamepadState;
GLFWgamepadstate Input::m_PrevGamepadState;

void Input::SetCallbacks(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
}

void Input::Update(float deltaTime)
{
	memset(m_KeysDown, false, sizeof(m_KeysDown));
	memset(m_KeysUp, false, sizeof(m_KeysUp));

	memset(m_MouseButtonsDown, false, sizeof(m_MouseButtonsDown));
	memset(m_MouseButtonsUp, false, sizeof(m_MouseButtonsUp));

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

bool Input::TurnCameraLeft()
{
	return KeyHeld(KEY_Q) || ButtonHeld(GAMEPAD_BUTTON_LEFT_BUMPER);
}
bool Input::TurnCameraRight()
{
	return KeyHeld(KEY_E) || ButtonHeld(GAMEPAD_BUTTON_RIGHT_BUMPER);
}

// Keyboard

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		m_KeysDown[key] = true;
		m_KeysHeld[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		m_KeysUp[key] = true;
		m_KeysHeld[key] = false;
	}
}

bool Input::KeyDown(int key)
{
	return m_KeysDown[key];
}
bool Input::KeyHeld(int key)
{
	return m_KeysHeld[key];
}
bool Input::KeyUp(int key)
{
	return m_KeysUp[key];
}

// Mouse

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		m_MouseButtonsDown[button] = true;
		m_MouseButtonsHeld[button] = true;
	}
	if (action == GLFW_RELEASE)
	{
		m_MouseButtonsUp[button] = true;
		m_MouseButtonsHeld[button] = false;
	}
}

bool Input::MouseButtonDown(int button)
{
	return m_MouseButtonsDown[button];
}
bool Input::MouseButtonHeld(int button)
{
	return m_MouseButtonsHeld[button];
}
bool Input::MouseButtonUp(int button)
{
	return m_MouseButtonsUp[button];
}

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	m_CursorX = xpos;
	m_CursorY = ypos;
}


glm::vec2 Input::MousePosition()
{
	return glm::vec2(m_CursorX, m_CursorY);
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