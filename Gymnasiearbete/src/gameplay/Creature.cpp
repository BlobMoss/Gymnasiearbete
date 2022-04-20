#include "Creature.h"

Creature::Creature()
	: m_FlashColor(glm::vec4(1.0f, 0.5f, 0.5f, 1.0f)), m_Health(100)
{

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

	Body::Update(deltaTime);
}

void Creature::GetHit(int damage, glm::vec3 knockBack)
{
	m_Health -= damage;

	m_KnockBackVelocity += knockBack;

	m_FlashTime = m_FlashDuration;
}

void Creature::Die()
{
	Remove();
}