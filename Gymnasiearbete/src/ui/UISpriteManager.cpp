#include "UISpriteManager.h"

#include "Inventory.h"

std::vector<UISprite*> UISpriteManager::m_UISprites;

void UISpriteManager::AddSprite(UISprite* uiSprite)
{
	m_UISprites.push_back(uiSprite);
}

void UISpriteManager::Update(float deltaTime)
{
	for (auto& uiSprite : m_UISprites)
	{
		if (uiSprite->m_Active)
		{
			uiSprite->Update(deltaTime);
		}
	}

	// Remove sprites that "WillBeRemoved"
	for (auto& uiSprite : m_UISprites)
	{
		if (uiSprite->WillBeRemoved() == true)
		{
			// Free pointer and set it to nullptr in m_Sprites
			delete uiSprite;
			uiSprite = nullptr;
		}
	}

	// Erase all pointers that have been freed and set to nullptr
	m_UISprites.erase(std::remove(m_UISprites.begin(), m_UISprites.end(), nullptr), m_UISprites.end());
}

void UISpriteManager::Draw()
{
	for (auto& uiSprite : m_UISprites)
	{
		if (uiSprite->m_Active)
		{
			uiSprite->Draw();
		}
	}
}