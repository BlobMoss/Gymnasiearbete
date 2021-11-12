#include "SpriteManager.h"

#include "Model.h"
#include <iostream>
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
	for (const auto& sprite : m_Sprites)
	{
		sprite->Update(deltaTime);
	}

	for (auto& sprite : m_Sprites)
	{
		if (sprite->WillBeRemoved() == true)
		{
			delete sprite;
			sprite = nullptr;
		}
	}
	m_Sprites.erase(std::remove(m_Sprites.begin(), m_Sprites.end(), nullptr), m_Sprites.end());
}
void SpriteManager::Draw(const Renderer &renderer)
{
	for (const auto& sprite : m_Sprites)
	{
		sprite->Draw(renderer);
	}
}