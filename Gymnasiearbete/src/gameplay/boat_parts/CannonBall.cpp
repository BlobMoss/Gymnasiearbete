#include "CannonBall.h"

CannonBall::CannonBall()
{
	m_Model = new Model("res/models/cannonball.obj", "res/textures/cannonball.png", "res/shaders/detailed.shader");
	m_ParticleModel = new Model("res/models/cannonball.obj", "res/textures/cannonball_particle.png", "res/shaders/standard_detailed.shader");

	m_Model->m_Highlighted = true;
	m_Model->m_HighlightColor = glm::vec4(0.950, 0.376, 0.24, 1.0f);
	m_ParticleModel->m_Highlighted = true;
	m_ParticleModel->m_HighlightColor = m_Model->m_HighlightColor;

	m_Gravity = 0.0f;
	m_ColliderRadius = 0.4f;
}
CannonBall::~CannonBall()
{

}

bool isNegative(fireParticle p)
{
	return p.scale <= 0.0f;
};

void CannonBall::Update(float deltaTime)
{
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

		m_Particles.push_back({ particlePosition, (randf() + 2.0f) / 2.5f });

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

	if (m_Position.y <= -1.5f)
	{
		m_Particles.clear();
		Remove();
	}

	Body::Update(deltaTime);
}

void CannonBall::Draw()
{
	for (auto& particle : m_Particles)
	{
		m_ParticleModel->Draw(particle.position, glm::vec3(0.0f), glm::vec3(particle.scale));
	}

	Body::Draw();
}