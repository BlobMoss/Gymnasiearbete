#include "Player.h"

#include "../Client.h"

#include "../graphics/Colors.h"

Player::Player()
	: m_Client(nullptr), m_MoveSpeed(5.0f)
{
	m_Model = new Model("res/models/sailor.obj", "res/textures/sailor_texture.png", "res/shaders/sailor.shader");
}
Player::Player(Client* client)
	: m_Client(client), m_MoveSpeed(5.0f)
{
	m_Model = new Model("res/models/sailor.obj", "res/textures/sailor_texture.png", "res/shaders/sailor.shader");

	float offsetRand = randf();
	m_BeardColor = Colors::HSVtoRGB(randf() * 45, 50, randf() * 75);
	m_CoatColor = Colors::HSVtoRGB(randf() * 360, 30, 30 + randf() * 10);
	m_HatColor = Colors::HSVtoRGB(randf() * 35, 30, 30 + randf() * 30);
}
Player::~Player()
{

}

void Player::Update(float deltaTime)
{
	if (m_Client != nullptr)
	{
		glm::vec3 movement = glm::vec3(Input::Horizontal(), 0.0f, Input::Vertical());

		if (glm::length(movement) > 1.0f) movement = glm::normalize(movement);

		float angle = Camera::m_ViewAngle;

		glm::mat4 rotationMat(1.0f);
		rotationMat = glm::rotate(rotationMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		movement = glm::vec3(rotationMat * glm::vec4(movement, 1.0));

		m_Velocity.x = movement.x * m_MoveSpeed;
		m_Velocity.z = movement.z * m_MoveSpeed;

		if (glm::length(movement) > 0.0f)
		{
			m_Rotation.y = -glm::atan(movement.z / movement.x);
			if (movement.x < 0.0f) m_Rotation.y += glm::pi<float>();
		}
	}

	if (glm::length(glm::vec3(m_Velocity.x, 0.0f, m_Velocity.z)) > 0.0f)
	{
		m_WalkAnim = true;
	}
	else
	{
		if (glm::mod(m_WalkTime, glm::pi<float>() / 13.0f) > glm::mod(m_WalkTime + deltaTime, glm::pi<float>() / 13.0f)) m_WalkAnim = false;
	}
	if (m_WalkAnim) m_WalkTime += deltaTime;

	m_Velocity.y += -m_Gravity * deltaTime;
	if (m_Position.y == 0.0f) m_Velocity.y = 0.0f;
}

void Player::Draw()
{
	if (m_Model == nullptr) return;

	m_Model->m_Shader.Bind();

	m_Model->m_Shader.SetUniform4f("u_BeardColor", m_BeardColor.r, m_BeardColor.g, m_BeardColor.b, 1.0f);
	m_Model->m_Shader.SetUniform4f("u_CoatColor", m_CoatColor.r, m_CoatColor.g, m_CoatColor.b, 1.0f);
	m_Model->m_Shader.SetUniform4f("u_HatColor", m_HatColor.r, m_HatColor.g, m_HatColor.b, 1.0f);

	m_Model->Draw(m_Position + glm::vec3(0.0f, glm::abs(glm::sin(m_WalkTime * 13.0f) * 0.15f), 0.0f), m_Rotation, m_Scale);
}

void Player::Move(float deltaTime)
{
	if (m_Client != nullptr)
	{
		Body::Move(deltaTime);
	}
}

void Player::SetDescription(std::vector<uint8_t>& desc)
{
	desc >> m_WillBeRemoved >> m_HatColor >> m_CoatColor >> m_BeardColor >> m_Scale >> m_Rotation >> m_Position;
}
std::vector<uint8_t> Player::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_BeardColor << m_CoatColor << m_HatColor << m_WillBeRemoved;
	return desc;
}
