#include "RespawnMenu.h"

#include "UISpriteManager.h"
#include "../gameplay/SpriteManager.h"

RespawnMenu* RespawnMenu::m_Instance;

RespawnMenu::RespawnMenu()
{
	m_Instance = this;

	m_Fade = new UISprite(new Image("res/images/black.png"));
	m_Fade->m_Scale = glm::ivec2(referenceWidth, referenceHeight);
	m_Fade->m_Color = glm::vec4(0.0f, 0.0f, 0.0f, 0.4f);
	m_Fade->m_SortingOrder = 0.95f;
	m_Fade->m_Active = false;
	UISpriteManager::AddSprite(m_Fade);

	unsigned int width, height;

	m_YouDied = new UISprite(new Image("res/images/dummy_image.png"));
	width = m_YouDied->m_Image->GetWidth();
	height = m_YouDied->m_Image->GetHeight();
	m_YouDied->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + 80);
	m_YouDied->m_SortingOrder = 0.96f;
	m_YouDied->m_Active = false;
	UISpriteManager::AddSprite(m_YouDied);

	m_RespawnButton = new UIButton(new Image("res/images/dummy_image.png"));
	width = m_RespawnButton->m_Image->GetWidth();
	height = m_RespawnButton->m_Image->GetHeight();
	m_RespawnButton->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + 0);
	m_RespawnButton->m_SortingOrder = 0.96f;
	m_RespawnButton->m_Active = false;
	UISpriteManager::AddSprite(m_RespawnButton);
}
RespawnMenu::~RespawnMenu()
{

}

void RespawnMenu::Update(float deltaTime)
{
	m_Fade->m_Active = false;
	m_YouDied->m_Active = false;
	m_RespawnButton->m_Active = false;

	if (m_Player == nullptr)
	{
		m_Player = SpriteManager::m_Player;
	}
	else
	{
		if (m_Player->m_Dead || m_Respawning)
		{
			m_Fade->m_Active = true;

			if (!m_Respawning)
			{
				m_YouDied->m_Active = true;
				m_RespawnButton->m_Active = true;

				if (m_RespawnButton->Down())
				{
					m_Respawning = true;
				}
			}
			else
			{
				if (m_Fade->m_Color.a >= 1.1f)
				{
					m_Player->Respawn();

					m_Respawned = true;
				}
				if (m_Fade->m_Color.a <= 0.0f)
				{
					m_Respawning = false;
					m_Respawned = false;
					m_Fade->m_Color.a = 0.4f;
					m_Fade->m_Active = false;
				}

				if (!m_Respawned)
				{
					m_Fade->m_Color.a += deltaTime * 0.5f;
				}
				else
				{
					m_Fade->m_Color.a -= deltaTime * 0.5f;
				}
			}
		}
	}
}