#include "SpriteManager.h"

#include "../Client.h"

#include "Collision.h"

#include "../graphics/Model.h"

Client* SpriteManager::m_Client = nullptr;

std::unordered_map<int64_t, Sprite*> SpriteManager::m_Sprites;
std::unordered_map<int64_t, std::vector<uint8_t>> SpriteManager::m_LastDescriptions;

int64_t SpriteManager::m_LocalIDCounter = -1;

std::vector<Body*> SpriteManager::m_Bodies;
std::vector<BlockGroup*> SpriteManager::m_BlockGroups;


// Add sprite to each type of list
void SpriteManager::AddSpriteInternal(int64_t id, Sprite* sprite)
{
	m_Sprites.insert_or_assign(id, sprite);

	if (dynamic_cast<Body*>(sprite) != nullptr)
	{
		m_Bodies.push_back(dynamic_cast<Body*>(sprite));
	}
	if (dynamic_cast<BlockGroup*>(sprite) != nullptr)
	{
		m_BlockGroups.push_back(dynamic_cast<BlockGroup*>(sprite));
	}
}

// Add new sprite locally
void SpriteManager::AddSpriteLocally(Sprite* sprite)
{
	int64_t id = m_LocalIDCounter;
	m_LocalIDCounter--;

	AddSpriteInternal(id, sprite);
}

// Add new sprite locally and tell other clients to do the same
void SpriteManager::AddSprite(Sprite* sprite)
{
	int64_t waitingID = m_LocalIDCounter;
	m_LocalIDCounter--;

	AddSpriteInternal(waitingID, sprite);

	net::message<MsgTypes> msg;
	msg.header.type = MsgTypes::Game_AddSprite;
	msg.body = sprite->GetDescription();
	msg << sprite->GetType() << waitingID;

	m_Client->Send(msg);
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
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		AddSpriteInternal(id, sprite);
	}
	break;
	case SpriteTypes::Body:
	{
		Body* sprite = new Body();
		sprite->m_Model = new Model("res/models/gem.obj", "res/textures/gem_texture.png", "res/shaders/lighting.shader");
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		AddSpriteInternal(id, sprite);
	}
	break;
	case SpriteTypes::Player:
	{
		Player* sprite = new Player();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		AddSpriteInternal(id, sprite);
	}
	break;
	case SpriteTypes::BlockGroup:
	{
		BlockGroup* sprite = new BlockGroup();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		AddSpriteInternal(id, sprite);
	}
	break;
	}
}

void SpriteManager::AssignID(int64_t oldID, int64_t newID)
{
	m_Sprites.insert_or_assign(newID, m_Sprites[oldID]);
	m_Sprites.erase(oldID);
	m_Sprites[newID]->m_Id = newID;
}

void SpriteManager::MakeOwner(int64_t id)
{
	m_Sprites[id]->m_OwnedHere = true;
}

void SpriteManager::AddSpriteWithID(int64_t id, Sprite* sprite)
{
	AddSpriteInternal(id, sprite);

	net::message<MsgTypes> msg;
	msg.header.type = MsgTypes::Game_AddSpriteWithID;
	msg.body = sprite->GetDescription();
	msg << sprite->GetType() << id;

	m_Client->Send(msg);
	// desc + type + id
}

void SpriteManager::RemoveSpriteByID(int64_t id)
{
	m_Sprites[id]->Remove();
}

void SpriteManager::UpdateLocally(float deltaTime)
{
	for (auto& sprite : m_Sprites)
	{
		sprite.second->Update(deltaTime);
	}

	// Handle different types of collisions
	for (unsigned int a = 0; a < m_Bodies.size(); a++)
	{
		for (unsigned int b = a; b < m_Bodies.size(); b++)
		{
			if (a != b)
			{
				Collision::CircleToCircle(m_Bodies[a], m_Bodies[b], deltaTime);
			}
		}
	}

	for (unsigned int a = 0; a < m_Bodies.size(); a++)
	{
		for (unsigned int b = 0; b < m_BlockGroups.size(); b++)
		{
			Collision::CircleToBlocks(m_Bodies[a], m_BlockGroups[b], deltaTime);
		}
	}

	for (unsigned int a = 0; a < m_BlockGroups.size(); a++)
	{
		for (unsigned int b = a; b < m_BlockGroups.size(); b++)
		{
			if (a != b)
			{
				Collision::BlocksToBlocks(m_BlockGroups[a], m_BlockGroups[b], deltaTime);
			}
		}
	}

	// Apply velocity after restricting it with collisions
	for (auto& body : m_Bodies)
	{
		body->Move();
	}
	for (auto& blockGroup : m_BlockGroups)
	{
		blockGroup->Move();
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
			// Only update sprites owned by this client
			if (sprite.second->m_OwnedHere)
			{
				net::message<MsgTypes> msg;
				msg.body = sprite.second->GetDescription();
				if (msg.body != m_LastDescriptions[id])
				{
					msg.header.type = MsgTypes::Game_UpdateSprite;
					msg << sprite.second->GetType() << id;

					m_Client->Send(msg);
					// desc + id
				}
			}
		}
	}

	for (auto& body : m_Bodies)
	{
		if (body->WillBeRemoved())
		{
			body = nullptr;
		}
	}
	m_Bodies.erase(std::remove(m_Bodies.begin(), m_Bodies.end(), nullptr), m_Bodies.end());

	for (auto it = m_Sprites.begin(); it != m_Sprites.end();)
	{
		if (it->second->WillBeRemoved())
		{
			int64_t id = it->first;
			delete m_Sprites[id];
			m_Sprites[id] = nullptr;
			it = m_Sprites.erase(it);
			m_LastDescriptions.erase(id);
		}
		else
			++it;
	}
}

void SpriteManager::ForceUpdate(uint64_t id)
{
	net::message<MsgTypes> msg;
	msg.body = m_Sprites[id]->GetDescription();
	msg.header.type = MsgTypes::Game_UpdateSprite;
	msg << m_Sprites[id]->GetType() << id;

	m_Client->Send(msg);
	// desc + id
}

void SpriteManager::SyncSprite(int64_t id, std::vector<uint8_t> desc)
{
	if (m_Sprites.find(id) != m_Sprites.end())
	{
		m_Sprites[id]->SetDescription(desc);
		m_LastDescriptions.insert_or_assign(id, desc);
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