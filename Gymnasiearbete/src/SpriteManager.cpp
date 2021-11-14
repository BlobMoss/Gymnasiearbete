#include "SpriteManager.h"

#include "Model.h"

SpriteManager::SpriteManager()
{
	
}
SpriteManager::~SpriteManager()
{

}

void SpriteManager::AddSprite(Sprite* sprite)
{
	m_Sprites.push_back(sprite);
}

void SpriteManager::Update(float deltaTime)
{
	// Update every sprite
	for (const auto& sprite : m_Sprites)
	{
		sprite->Update(deltaTime);
	}

	// Remove sprites that "WillBeRemoved"
	for (auto& sprite : m_Sprites)
	{
		if (sprite->WillBeRemoved() == true)
		{
			// Free pointer and set it to nullptr in m_Sprites
			delete sprite;
			sprite = nullptr;
		}
	}
	// Erase all pointers that have been freed and set to nullptr
	m_Sprites.erase(std::remove(m_Sprites.begin(), m_Sprites.end(), nullptr), m_Sprites.end());
}
void SpriteManager::Draw(Camera& camera)
{
	// Draw every sprite
	for (const auto& sprite : m_Sprites)
	{
		sprite->Draw(camera);
	}
}