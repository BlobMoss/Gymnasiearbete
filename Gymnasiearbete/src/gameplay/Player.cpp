#include "Player.h"

#include "../Client.h"

Player::Player()
	: m_Client(nullptr), m_MoveSpeed(5.0f)
{
	m_Model = new Model("res/models/sailor.obj", "res/textures/sailor_texture.png", "res/shaders/detailed.shader");
}
Player::Player(Client* client)
	: m_Client(client), m_MoveSpeed(5.0f)
{
	m_Model = new Model("res/models/sailor.obj", "res/textures/sailor_texture.png", "res/shaders/detailed.shader");
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

		float angle = Camera::GetViewAngle();

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
	
	Body::Update(deltaTime);
}

void Player::Draw()
{
	Body::Draw();
}

void Player::SetDescription(std::vector<uint8_t>& desc)
{
	desc >> m_WillBeRemoved >> m_Scale >> m_Rotation >> m_Position;
}
std::vector<uint8_t> Player::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_WillBeRemoved;
	return desc;
}