#include "Cannon.h"

#include "../SpriteManager.h"

Cannon::Cannon()
{
	m_Model = new Model("res/models/cannon.obj", "res/textures/cannon.png", "res/shaders/detailed.shader");
	m_FuseModel = new Model("res/models/fuse.obj", "res/textures/fuse.png", "res/shaders/detailed.shader");

	m_ColliderRadius = 0.45f;
}
Cannon::~Cannon()
{

}

void Cannon::Update(float deltaTime)
{
	if (m_LoadTime <= 0.0f)
	{
		m_Loaded = true;
	}
	m_LoadTime -= deltaTime;

	m_FuseModel->m_Highlighted = m_Model->m_Highlighted;

	BoatPart::Update(deltaTime);
}

void Cannon::Draw()
{
	// Draw fuse to indicate loaded
	if (m_Loaded) m_FuseModel->Draw(m_Position, m_Rotation, m_Scale);
	else m_Model->m_Highlighted = false;

	BoatPart::Draw();
}

void Cannon::Fire()
{
	if (m_Loaded)
	{
		CannonBall* newBall = new CannonBall();

		glm::vec3 offset(
			1.0f * glm::cos(-m_Rotation.y + glm::pi<float>() / 2.0f),
			-0.5f,
			1.0f * glm::sin(-m_Rotation.y + glm::pi<float>() / 2.0f)
		);
		newBall->m_Position = m_Position + offset;
		newBall->m_PotentialPosition = m_Position + offset;
		newBall->m_StartVelocity = m_Velocity;
		newBall->m_Rotation = m_Rotation;

		SpriteManager::AddSprite(newBall);

		m_LoadTime = m_LoadDelay;
		m_Loaded = false;
	}
}

void Cannon::SetDescription(std::vector<uint8_t>& desc)
{
	bool prevLoaded = m_Loaded;
	desc >> m_WillBeRemoved >> m_Loaded >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
	if (prevLoaded && !m_Loaded) m_LoadTime = m_LoadDelay;
}
std::vector<uint8_t> Cannon::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << m_Position << m_Rotation << m_Scale << m_Velocity << m_Loaded << m_WillBeRemoved;
	return desc;
}