#include "SpriteManager.h"

#include "Model.h"

SpriteManager::SpriteManager()
{
	
}
SpriteManager::~SpriteManager()
{

}

void SpriteManager::AddSprite(Sprite &sprite)
{
	m_Sprites.push_back(&sprite);
}

void SpriteManager::Update(float deltaTime)
{
	for (const auto& sprite : m_Sprites)
	{
		sprite->Update(deltaTime);
	}
}
void SpriteManager::Draw(Renderer renderer)
{
	for (const auto& sprite : m_Sprites)
	{
		sprite->Draw(renderer);
	}
}