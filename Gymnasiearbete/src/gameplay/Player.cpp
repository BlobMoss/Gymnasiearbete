#include "Player.h"

#include "../graphics/Colors.h"

#include "SpriteManager.h"
#include "Raycast.h"

Player::Player()
{
	m_Model = new Model("res/models/sailor.obj", "res/textures/sailor.png", "res/shaders/sailor.shader");

	float offsetRand = randf();
	m_BeardColor = Colors::HSVtoRGB(randf() * 45, 50, randf() * 75);
	m_CoatColor = Colors::HSVtoRGB(randf() * 360, 30, 30 + randf() * 10);
	m_HatColor = Colors::HSVtoRGB(randf() * 35, 30, 30 + randf() * 30);
}
Player::~Player()
{
	delete m_BlockCursor;
}

void Player::Update(float deltaTime)
{
	std::cout << m_Position.y << std::endl;

	if (m_OwnedHere)
	{
		// Cursor
		if (m_BlockCursor == nullptr)
		{
			m_BlockCursor = new BlockCursor();
			SpriteManager::AddSpriteLocally(m_BlockCursor);
		}

		if (Input::KeyDown(KEY_G))
		{
			Mast* mast = new Mast();
			mast->m_Position = m_BlockCursor->m_Position;
			SpriteManager::AddSprite(mast);
		}

		RayHit hit = Camera::RayFromScreen(Input::MousePosition());

		m_BlockCursor->m_Visable = false;
		if (hit.blockGroup != nullptr)
		{
			m_BlockCursor->SetTransform(hit);
			if (glm::distance(m_Position, glm::vec3(m_BlockCursor->m_Position)) <= m_PlacementRange)
			{
				m_BlockCursor->m_Visable = true;
			}
		}
		if (m_BlockCursor->m_Selected.blockGroup != nullptr)
		{
			if (glm::distance(m_Position, glm::vec3(m_BlockCursor->m_Position)) <= m_PlacementRange)
			{
				m_BlockCursor->m_Visable = true;
			}
		}
		
		// Walking
		m_Movement = glm::vec3(Input::Horizontal(), 0.0f, Input::Vertical());

		if (glm::length(m_Movement) > 1.0f) m_Movement = glm::normalize(m_Movement);

		float angle = Camera::m_ViewAngle;

		glm::mat4 rotationMat(1.0f);
		rotationMat = glm::rotate(rotationMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		m_Movement = glm::vec3(rotationMat * glm::vec4(m_Movement, 1.0));

		m_Velocity.x = m_Movement.x * m_MoveSpeed * float(m_Grounded ? 1.0f : 0.5f);
		m_Velocity.z = m_Movement.z * m_MoveSpeed * float(m_Grounded ? 1.0f : 0.5f);

		if (glm::length(m_Movement) > 0.0f)
		{
			m_Rotation.y = -glm::atan(m_Movement.z / m_Movement.x);
			if (m_Movement.x < 0.0f) m_Rotation.y += glm::pi<float>();
		}

		// Climbing
		if (climbing && !climbBlocked) m_Velocity.y = 5.0f;
		climbing = false;
		climbBlocked = false;
	}

	if (glm::length(m_Movement) > 0.0f && m_Grounded)
	{
		m_WalkAnim = true;
	}
	else
	{
		if (glm::mod(m_WalkTime, glm::pi<float>() / 13.0f) > glm::mod(m_WalkTime + deltaTime, glm::pi<float>() / 13.0f)) m_WalkAnim = false;
	}
	if (m_WalkAnim) m_WalkTime += deltaTime;

	Body::Update(deltaTime);
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

void Player::OnCollision(Body* body)
{
	if (dynamic_cast<DroppedItem*>(body) != nullptr)
	{
		body->Remove();
	}
}

void Player::OnCollision(BlockGroup* blockGroup, BlockCollisions side)
{
	if (side == BlockCollisions::Side)
	{
		if (glm::length(m_Movement) > 0.0f)
		{
			climbing = true;
		}
	}
	if (side == BlockCollisions::BlockAbove)
	{
		climbBlocked = true;
	}

	Body::OnCollision(blockGroup, side);
}

void Player::SetDescription(std::vector<uint8_t>& desc)
{
	desc >> m_WillBeRemoved >> m_HatColor >> m_CoatColor >> m_BeardColor >> m_Movement >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
}
std::vector<uint8_t> Player::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_Velocity << m_Movement << m_BeardColor << m_CoatColor << m_HatColor << m_WillBeRemoved;
	return desc;
}
