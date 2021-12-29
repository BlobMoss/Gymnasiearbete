#include "SpriteManager.h"

#include "../Client.h"

#include "../graphics/Model.h"

Client* SpriteManager::client = nullptr;

std::unordered_map<int64_t, Sprite*> SpriteManager::m_Sprites;
std::unordered_map<int64_t, std::vector<uint8_t>> SpriteManager::m_LastDescriptions;


int64_t SpriteManager::m_LocalIDCounter = -1;

// Add new sprite locally and tell other clients to do the same
void SpriteManager::AddSprite(Sprite* sprite)
{
	int64_t waitingID = m_LocalIDCounter;
	m_LocalIDCounter--;

	m_Sprites.insert_or_assign(waitingID, sprite);

	net::message<MsgTypes> msg;
	msg.header.type = MsgTypes::Game_AddSprite;
	msg.body = sprite->GetDescription();
	msg << sprite->GetType() << waitingID;

	client->Send(msg);
	// desc + type + id
}

// Add sprite created by another client
void SpriteManager::AddSprite(int64_t id, SpriteTypes type, std::vector<uint8_t> desc)
{
	switch (type)
	{
	case SpriteTypes::Sprite:
	{
		Sprite* sprite = new Sprite(new Model("res/models/gem.obj", "res/textures/gem_texture.png", "res/shaders/lighting.shader"));
		sprite->SetDescription(desc);
		m_Sprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::BlockGroup:
	{
		BlockGroup* sprite = new BlockGroup();
		sprite->SetDescription(desc);
		m_Sprites.insert_or_assign(id, sprite);
	}
	break;
	}
}

// 
void SpriteManager::AddSpriteWithID(int64_t id, Sprite* sprite)
{
	m_Sprites.insert_or_assign(id, sprite);

	net::message<MsgTypes> msg;
	msg.header.type = MsgTypes::Game_AddSpriteWithID;
	msg.body = sprite->GetDescription();
	msg << sprite->GetType() << id;

	client->Send(msg);
	// desc + type + id
}

// Give common ID to local sprite
void SpriteManager::AssignID(int64_t oldID, int64_t newID)
{
	m_Sprites.insert_or_assign(newID, m_Sprites[oldID]);
	m_Sprites.erase(oldID);
}

void SpriteManager::RemoveSpriteByID(int64_t id)
{
	m_Sprites[id]->Remove();
}

void SpriteManager::UpdateLocally(float deltaTime)
{
	// Update every sprite
	for (auto& sprite : m_Sprites)
	{
		sprite.second->Update(deltaTime);
	}
}

void SpriteManager::SaveDescriptions()
{
	for (auto& sprite : m_Sprites)
	{
		int64_t id = sprite.first;
		std::vector<uint8_t> desc = sprite.second->GetDescription();
		m_LastDescriptions.insert_or_assign(id, desc);
	}
}

void SpriteManager::UpdateServer()
{
	for (auto& sprite : m_Sprites)
	{
		int64_t id = sprite.first;
		// Do not update waiting local sprites for other clients
		if (id > 0)
		{
			net::message<MsgTypes> msg;
			msg.body = sprite.second->GetDescription();
			if (msg.body != m_LastDescriptions[id])
			{
				msg.header.type = MsgTypes::Game_UpdateSprite;
				msg << sprite.second->GetType() << id;

				client->Send(msg);
				// desc + id
			}
		}
	}

	for (auto it = m_Sprites.begin(); it != m_Sprites.end();)
	{
		if (it->second->WillBeRemoved())
		{
			int64_t id = it->first;
			delete m_Sprites[id];
			it = m_Sprites.erase(it);
			m_LastDescriptions.erase(id);
		}
		else
			++it;
	}
}

void SpriteManager::SyncSprite(int64_t id, std::vector<uint8_t> desc)
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