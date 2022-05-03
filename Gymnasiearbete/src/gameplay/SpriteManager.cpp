#include "SpriteManager.h"

#include "../Client.h"

#include "Collision.h"

#include "World.h"

#include "../graphics/Model.h"

#include "../ui/Inventory.h"
#include "../ui/Crafting.h"
#include "../ui/HealthBar.h"
#include "../ui/RespawnMenu.h"

Client* SpriteManager::m_Client = nullptr;

std::unordered_map<int64_t, Sprite*> SpriteManager::m_Sprites;
std::unordered_map<int64_t, std::vector<uint8_t>> SpriteManager::m_LastDescriptions;

bool SpriteManager::m_Updating;
std::unordered_map<int64_t, Sprite*> SpriteManager::m_TempSprites;

int64_t SpriteManager::m_LocalIDCounter = -1;

Player* SpriteManager::m_Player = nullptr;
std::vector<Body*> SpriteManager::m_Bodies;
std::vector<Creature*> SpriteManager::m_Creatures;
std::vector<BlockGroup*> SpriteManager::m_BlockGroups;
std::vector<BoatPart*> SpriteManager::m_BoatParts;

bool SpriteManager::drawingShadows;

// Add sprite to each type of list
void SpriteManager::AddSpriteInternal(int64_t id, Sprite* sprite)
{
	m_Sprites.insert_or_assign(id, sprite);

	if (dynamic_cast<Player*>(sprite) != nullptr)
	{
		Player* player = dynamic_cast<Player*>(sprite);
		if (player->m_OwnedHere) m_Player = player;
	}
	if (dynamic_cast<Body*>(sprite) != nullptr)
	{
		m_Bodies.push_back(dynamic_cast<Body*>(sprite));
	}
	if (dynamic_cast<Creature*>(sprite) != nullptr)
	{
		m_Creatures.push_back(dynamic_cast<Creature*>(sprite));
	}
	if (dynamic_cast<BlockGroup*>(sprite) != nullptr)
	{
		m_BlockGroups.push_back(dynamic_cast<BlockGroup*>(sprite));
	}
	if (dynamic_cast<BoatPart*>(sprite) != nullptr)
	{
		m_BoatParts.push_back(dynamic_cast<BoatPart*>(sprite));
	}
}

// Add new sprite locally
void SpriteManager::AddSpriteLocally(Sprite* sprite)
{
	int64_t id = m_LocalIDCounter;
	m_LocalIDCounter--;

	if (!m_Updating)
		AddSpriteInternal(id, sprite);
	else
		m_TempSprites.insert_or_assign(id, sprite);
}

// Add new sprite and tell other clients to do the same
void SpriteManager::AddSprite(Sprite* sprite)
{
	int64_t waitingID = m_LocalIDCounter;
	m_LocalIDCounter--;

	if (!m_Updating)
		AddSpriteInternal(waitingID, sprite);
	else
		m_TempSprites.insert_or_assign(waitingID, sprite);

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
		Sprite* sprite = new Sprite();
		sprite->SetDescription(desc);
		sprite->m_Model = new Model(sprite->m_ObjPath, sprite->m_TexturePath, sprite->m_ShaderPath);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::Body:
	{
		Body* sprite = new Body();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::Player:
	{
		Player* sprite = new Player();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::BlockGroup:
	{
		BlockGroup* sprite = new BlockGroup();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::DroppedItem:
	{
		DroppedItem* sprite = new DroppedItem(-1, 1);
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::Mast:
	{
		Mast* sprite = new Mast();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::Helm:
	{
		Helm* sprite = new Helm();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::Cannon:
	{
		Cannon* sprite = new Cannon();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::CannonBall:
	{
		CannonBall* sprite = new CannonBall();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
	}
	break;
	case SpriteTypes::Bullet:
	{
		Bullet* sprite = new Bullet();
		sprite->SetDescription(desc);
		sprite->m_OwnedHere = false;
		sprite->m_Id = id;
		m_TempSprites.insert_or_assign(id, sprite);
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
	if (!m_Updating)
		AddSpriteInternal(id, sprite);
	else
		m_TempSprites.insert_or_assign(id, sprite);

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
	m_Updating = true;

	if (World::m_Instance != nullptr) World::m_Instance->Update(deltaTime);

	if (Crafting::m_Instance != nullptr) Crafting::m_Instance->Update(deltaTime);
	if (Inventory::m_Instance != nullptr) Inventory::m_Instance->Update(deltaTime);
	if (HealthBar::m_Instance != nullptr) HealthBar::m_Instance->Update(deltaTime);
	if (RespawnMenu::m_Instance != nullptr) RespawnMenu::m_Instance->Update(deltaTime);

	for (auto& sprite : m_Sprites)
	{
		if (sprite.second->m_Active) sprite.second->Update(deltaTime);
	}

	// Handle different types of collisions
	for (unsigned int a = 0; a < m_Bodies.size(); a++)
	{
		for(unsigned int b = a; b < m_Bodies.size(); b++)
		{
			if (m_Bodies[a]->m_Active && m_Bodies[b]->m_Active && a != b)
			{
				Collision::CircleToCircle(m_Bodies[a], m_Bodies[b], deltaTime);
			}
		}
	}

	for (unsigned int a = 0; a < m_Bodies.size(); a++)
	{
		for (unsigned int b = 0; b < m_BlockGroups.size(); b++)
		{
			if (m_Bodies[a]->m_Active && m_BlockGroups[b]->m_Active)
			{
				Collision::CircleToBlocks(m_Bodies[a], m_BlockGroups[b], deltaTime);
			}
		}
	}

	for (unsigned int a = 0; a < m_BlockGroups.size(); a++)
	{
		for (unsigned int b = a; b < m_BlockGroups.size(); b++)
		{
			if (m_BlockGroups[a]->m_Active && m_BlockGroups[b]->m_Active && a != b)
			{
				Collision::BlocksToBlocks(m_BlockGroups[a], m_BlockGroups[b], deltaTime);
			}
		}
	}

	for (auto& blockGroup : m_BlockGroups)
	{
		if (blockGroup->m_Active)
		{
			Collision::BlocksToBorder(blockGroup, deltaTime);
		}
	}

	// Find boatPart closest to player
	BoatPart* closestPart = nullptr;
	float shortest = INFINITY;
	for (auto& boatPart : m_BoatParts)
	{
		if (boatPart->m_Occupied && !boatPart->m_OccupiedHere) continue;

		float d = glm::distance(m_Player->m_PotentialPosition, boatPart->m_PotentialPosition);

		if (d < shortest && d <= 2.0f)
		{
			shortest = d;
			closestPart = boatPart;
		}
	}
	if (closestPart != nullptr && m_Player->m_PotentialPosition.y == 0.0f)
	{
		closestPart->m_Highlighted = true;
		m_Player->m_InteractTarget = closestPart;
	}

	// Apply velocity after restricting it with collisions
	for (auto& body : m_Bodies)
	{
		if (body->m_Active) body->Move();
	}
	for (auto& blockGroup : m_BlockGroups)
	{
		if (blockGroup->m_Active) blockGroup->Move();
	}

	//

	m_Updating = false;

	for (auto& sprite : m_TempSprites)
	{
		AddSpriteInternal(sprite.first, sprite.second);
	}
	m_TempSprites.clear();
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

	for (auto& body : m_Bodies) if (body->WillBeRemoved()) body = nullptr;
	m_Bodies.erase(std::remove(m_Bodies.begin(), m_Bodies.end(), nullptr), m_Bodies.end());

	for (auto& creature : m_Creatures) if (creature->WillBeRemoved()) creature = nullptr;
	m_Creatures.erase(std::remove(m_Creatures.begin(), m_Creatures.end(), nullptr), m_Creatures.end());

	for (auto& blockGroup : m_BlockGroups) if (blockGroup->WillBeRemoved()) blockGroup = nullptr;
	m_BlockGroups.erase(std::remove(m_BlockGroups.begin(), m_BlockGroups.end(), nullptr), m_BlockGroups.end());

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
	msg.header.type = MsgTypes::Game_ForceUpdateSprite;
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

void SpriteManager::ForceSyncSprite(int64_t id, std::vector<uint8_t> desc)
{
	if (m_Sprites.find(id) != m_Sprites.end())
	{
		std::vector<uint8_t> descCopy = desc;
		m_Sprites[id]->ForcedSetDescription(desc);
		if (desc == descCopy)
		{
			m_Sprites[id]->SetDescription(desc);
		}
		m_LastDescriptions.insert_or_assign(id, desc);
	}
}

void SpriteManager::Draw()
{
	// lightSpace Matrix:
	glm::mat4 lightProjection, lightView;

	lightProjection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 0.0f, 65.0f);

	lightView = glm::lookAt(glm::vec3(20.0f, 30.0f, 0.0f) + Camera::m_TargetPosition, Camera::m_TargetPosition, glm::vec3(0.0f, 1.0f, 0.0f));

	Model::lightSpaceMat = lightProjection * lightView;

	// View Matrix:
	Model::viewMat = glm::mat4(1.0f);

	Model::viewMat = glm::rotate(Model::viewMat, Camera::m_Rotation.x, glm::vec3(1, 0, 0));
	Model::viewMat = glm::rotate(Model::viewMat, Camera::m_Rotation.y, glm::vec3(0, 1, 0));
	Model::viewMat = glm::rotate(Model::viewMat, Camera::m_Rotation.z, glm::vec3(0, 0, 1));
	Model::viewMat = glm::translate(Model::viewMat, -Camera::m_Position);

	// Draw every sprite
	for (const auto& sprite : m_Sprites)
	{
		if (sprite.second->m_Active && !sprite.second->m_Model->m_HasTransparency)
		{
			sprite.second->Draw();
		}
	}
	// Draw transparent sprites after
	for (const auto& sprite : m_Sprites)
	{
		if (sprite.second->m_Active && sprite.second->m_Model->m_HasTransparency)
		{
			sprite.second->Draw();
		}
	}
}
