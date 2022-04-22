#include "HealthBar.h"

#include "UISpriteManager.h"
#include "../gameplay/SpriteManager.h"

HealthBar* HealthBar::m_Instance;

HealthBar::HealthBar()
{
	m_Instance = this;

	m_BackGround = new UISprite(new Image("res/images/healthbar_background.png"));
	m_BackGround->m_Position = glm::ivec2(254, 57);
	m_BackGround->m_SortingOrder = 0.55f;
	UISpriteManager::AddSprite(m_BackGround);

	m_Bar = new UISprite(new Image("res/images/healthbar.png"));
	m_Bar->m_Position = glm::ivec2(275, 59);
	m_Bar->m_SortingOrder = 0.6f;
	UISpriteManager::AddSprite(m_Bar);
}
HealthBar::~HealthBar()
{

}

void HealthBar::Update(float deltaTime)
{
	m_BackGround->m_Active = false;
	
	if (m_Player == nullptr)
	{
		m_Player = SpriteManager::m_Player;
	}
	else
	{
		if (m_Player->m_Health < 100.0f && m_Player->m_Health > 0.0f)
		{
			m_BackGround->m_Active = true;

			m_Bar->m_Scale = glm::vec2(90 * (m_Player->m_Health / 100.0f), 1.0f);
		}
	}
	m_Bar->m_Active = m_BackGround->m_Active;
}