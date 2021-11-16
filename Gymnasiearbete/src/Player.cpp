#include "Player.h"
Player::Player()
	: m_MoveSpeed(5.0f)
{
	m_Model = new Model("res/models/gem.obj", "res/textures/gem_texture.png", "res/shaders/lighting.shader");
}
Player::~Player()
{

}

void Player::Update(float deltaTime)
{
	glm::vec3 movement = glm::vec3(Input::Horizontal(), 0.0f, Input::Vertical());

	if (glm::length(movement) > 1.0f) movement = glm::normalize(movement);

	float angle = Camera::GetViewAngle();

	glm::mat4 rotationMat(1.0f);
	rotationMat = glm::rotate(rotationMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	movement = glm::vec3(rotationMat * glm::vec4(movement, 1.0));

	m_Position += movement * m_MoveSpeed * deltaTime;

	if (glm::length(movement) > 0.0f) m_Rotation.y = -glm::atan(movement.z / movement.x);

	Sprite::Update(deltaTime);
}

void Player::Draw()
{
	Sprite::Draw();
}