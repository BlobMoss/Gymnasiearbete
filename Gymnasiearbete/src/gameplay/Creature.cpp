#include "Creature.h"

Creature::Creature()
	: m_FlashColor(glm::vec4(1.0f, 0.5f, 0.5f, 1.0f)), m_MaxHealth(100.0f)
{
	m_Health = m_MaxHealth;
}
Creature::~Creature()
{

}

void Creature::Update(float deltaTime)
{
	m_Color = m_FlashTime > 0.0f ? m_FlashColor : glm::vec4(1.0f);

	m_FlashTime -= deltaTime;

	if (!m_Dead && m_Health <= 0 && m_OwnedHere)
	{
		m_Dead = true;
		Die();
	}

	m_Health = std::min(100.0f, m_Health + deltaTime);

	Body::Update(deltaTime);
}

void Creature::GetHit(float damage, glm::vec3 knockBack)
{
	m_Health -= damage;

	m_KnockBackVelocity += knockBack;

	m_FlashTime = m_FlashDuration;
}

void Creature::Die()
{
	Remove();
}