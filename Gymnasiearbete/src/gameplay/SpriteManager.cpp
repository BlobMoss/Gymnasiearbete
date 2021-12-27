#include "SpriteManager.h"

#include "../Client.h"

#include "../graphics/Model.h"

Client* SpriteManager::client = nullptr;

std::unordered_map<int64_t, Sprite*> SpriteManager::m_Sprites;

int64_t SpriteManager::m_LocalIDCounter = -1;

SpriteManager::SpriteManager()
{
	
}
SpriteManager::~SpriteManager()
{

}

// Add new sprite locally and tell other clients to do the same
void SpriteManager::AddSprite(Sprite* sprite)
{
	int64_t tempID = m_LocalIDCounter;
	m_LocalIDCounter--;

	m_Sprites.insert_or_assign(tempID, sprite);

	net::message<MsgTypes> msg;
	msg.header.id = MsgTypes::Game_AddSprite;
	msg << tempID << sprite->GetDescription();
	client->Send(msg);
}

// Add sprite created by another client
void SpriteManager::AddSprite(int64_t id, Sprite* sprite)
{
	m_Sprites.insert_or_assign(id, sprite);
}

// Give common ID to local sprite
void SpriteManager::AssignID(int64_t oldID, int64_t newID)
{
	std::cout << "old: " << oldID << std::endl;
	std::cout << "new: " << newID << std::endl;
	m_Sprites.insert_or_assign(newID, m_Sprites[oldID]);
	m_Sprites.erase(oldID);
}

void SpriteManager::UpdateLocally(float deltaTime)
{
	// Update every sprite
	for (auto& sprite : m_Sprites)
	{
		sprite.second->Update(deltaTime);
	}

	// Remove sprites that "WillBeRemoved"
	for (auto& sprite : m_Sprites)
	{
		if (sprite.second->WillBeRemoved() == true)
		{
			delete sprite.second;
			m_Sprites.erase(sprite.first);
		}
	}
}
void SpriteManager::UpdateServer()
{
	for (auto& sprite : m_Sprites)
	{
		// Do not update local sprites on other clients
		if (sprite.first > 0)
		{
			net::message<MsgTypes> msg;
			msg.header.id = MsgTypes::Game_UpdateSprite;

			int64_t id = sprite.first;
			SpriteDescription desc = sprite.second->GetDescription();

			msg << id << desc;
			client->Send(msg);
		}
	}
}

void SpriteManager::SyncSprite(int64_t id, SpriteDescription desc)
{
	if (m_Sprites.find(id) != m_Sprites.end())
	{
		m_Sprites[id]->SetDescription(desc);
	}
}

void SpriteManager::Draw()
{
	// Draw every sprite
	for (const auto& sprite : m_Sprites)
	{
		sprite.second->Draw();
	}
}