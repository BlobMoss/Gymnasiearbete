#include "CannonBall.h"

#include "../SpriteManager.h"
#include "../Player.h"

Model* CannonBall::m_CannonBallModel = nullptr;
Model* CannonBall::m_ParticleModel = nullptr;

CannonBall::CannonBall()
{
	if (m_CannonBallModel == nullptr)
	{
		m_CannonBallModel = new Model("res/models/cannonball.obj", "res/textures/cannonball.png", "res/shaders/detailed.shader");
		m_CannonBallModel->m_HighlightColor = glm::vec4(0.950, 0.376, 0.24, 1.0f);
		m_CannonBallModel->m_DiffuseStrength = 2.0f;
	}
	m_Model = m_CannonBallModel;
	if (m_ParticleModel == nullptr)
	{
		m_ParticleModel = new Model("res/models/cannonball.obj", "res/textures/cannonball_particle.png", "res/shaders/standard_detailed.shader");
		m_ParticleModel->m_CastsShadow = false;
		m_ParticleModel->m_HighlightColor = m_CannonBallModel->m_HighlightColor;
	}

	m_Gravity = 0.0f;
	m_ColliderRadius = 0.4f;
	m_Highlighted = true;
	m_Static = true;
}
CannonBall::~CannonBall()
{
	m_Model = nullptr;
}

void CannonBall::OnCollision(Body* body)
{
	if (dynamic_cast<BoatPart*>(body) != nullptr)
	{
		dynamic_cast<Creature*>(body)->GetHit(1000, glm::vec3(0.0f));
	}
	else if (dynamic_cast<Creature*>(body) != nullptr)
	{
		glm::vec3 knockBack(m_Position - body->m_Position);
		dynamic_cast<Creature*>(body)->GetHit(90, glm::normalize(knockBack) * 6.0f);
	}
}
void CannonBall::OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side)
{
	unsigned char blockType = blockGroup->GetBlock(blockPos);

	blockGroup->BreakBlock(blockPos);

	if (blockPos.y == 1 || blockType == SAND || blockType == GRASS)
	{
		blockGroup->BreakBlock(glm::ivec3(blockPos.x, 0, blockPos.z));
		Remove();
		SpriteManager::ForceUpdate(m_Id);
	}
}

bool isNegative(fireParticle p)
{
	return p.scale <= 0.0f;
};

void CannonBall::Update(float deltaTime)
{
	m_LastDelta = deltaTime;

	m_Velocity = glm::vec3(
		m_StartVelocity.x + m_Speed * glm::cos(-m_Rotation.y + glm::pi<float>() / 2.0f),
		m_Velocity.y,
		m_StartVelocity.z + m_Speed * glm::sin(-m_Rotation.y + glm::pi<float>() / 2.0f)
	);

	if (m_FlyTime >= m_FlyDuration)
	{
		m_Gravity = 20.0f;
	}

	m_FlyTime += deltaTime;

	if (m_ParticleTime <= 0.0f)
	{
		glm::vec3 particlePosition = m_Position;

		particlePosition += glm::vec3(randf() - 0.5f, randf() - 0.5f, randf() - 0.5f) * 0.4f;

		m_Particles.push_back({ particlePosition + glm::vec3(0.0f, 1.0f, 0.0f), 1.0f });

		m_ParticleTime = m_ParticleDelay;
	}
	m_ParticleTime -= deltaTime;

	for (auto& particle : m_Particles)
	{
		particle.scale -= deltaTime * 5.0f;
		particle.scale = std::max(particle.scale, 0.0f);
	}

	std::vector<fireParticle>::iterator it;
	it = std::remove_if(m_Particles.begin(), m_Particles.end(), isNegative);

	if (m_Position.y <= -2.5f)
	{
		m_Particles.clear();
		Remove();
	}

	m_Velocity.y += -m_Gravity * deltaTime;

	m_PotentialPosition = m_Position + m_Velocity * deltaTime;

	Sprite::Update(deltaTime);
}

void CannonBall::Move()
{
	m_Position += m_Velocity * m_LastDelta;
}

void CannonBall::Draw()
{
	for (auto& particle : m_Particles)
	{
		m_ParticleModel->Draw(particle.position, glm::vec3(0.0f), glm::vec3(particle.scale), m_Color, true);
	}

	m_CannonBallModel->Draw(m_Position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), m_Scale, m_Color, m_Highlighted);
}