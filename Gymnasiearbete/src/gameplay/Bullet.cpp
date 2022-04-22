#include "Bullet.h"

#include "SpriteManager.h"
#include "Player.h"
#include "boat_parts/CannonBall.h"

Bullet::Bullet()
{
	if (CannonBall::m_CannonBallModel == nullptr)
	{
		CannonBall::m_CannonBallModel = new Model("res/models/cannonball.obj", "res/textures/cannonball.png", "res/shaders/detailed.shader");
		CannonBall::m_CannonBallModel->m_HighlightColor = glm::vec4(0.950, 0.376, 0.24, 1.0f);
		CannonBall::m_CannonBallModel->m_DiffuseStrength = 2.0f;
	}
	m_Model = CannonBall::m_CannonBallModel;
	m_Scale = glm::vec3(0.6f);
	
	m_Gravity = 0.0f;
	m_ColliderRadius = 0.4f;
	m_Highlighted = true;
	m_Static = true;
}
Bullet::~Bullet()
{
	m_Model = nullptr;
}

void Bullet::OnCollision(Body* body)
{
	if (dynamic_cast<Creature*>(body) != nullptr)
	{
		dynamic_cast<Creature*>(body)->GetHit(40, glm::normalize(m_Velocity) * 3.0f);

		Remove();
		SpriteManager::ForceUpdate(m_Id);
	}
}
void Bullet::OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side)
{
	Remove();
	SpriteManager::ForceUpdate(m_Id);
}

void Bullet::Update(float deltaTime)
{
	m_LastDelta = deltaTime;

	m_Velocity = glm::vec3(
		m_Speed * glm::cos(-m_Rotation.y + glm::pi<float>() / 2.0f),
		m_Velocity.y,
		m_Speed * glm::sin(-m_Rotation.y + glm::pi<float>() / 2.0f)
	);

	if (m_FlyTime >= m_FlyDuration)
	{
		m_Gravity = 20.0f;
	}

	m_FlyTime += deltaTime;

	m_Velocity.y += -m_Gravity * deltaTime;

	m_PotentialPosition = m_Position + m_Velocity * deltaTime;

	if (m_Position.y <= -1.5f)
	{
		Remove();
	}

	Sprite::Update(deltaTime);
}

void Bullet::Move()
{
	m_Position += m_Velocity * m_LastDelta;
}
