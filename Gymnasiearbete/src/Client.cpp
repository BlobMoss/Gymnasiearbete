#include "Client.h"
#include "graphics/Water.h"
#include "ui/Inventory.h"

uint32_t Client::m_ClientID = 0;

Client::Client()
	: waitingToConnect(true)
{
	SpriteManager::m_Client = this;
}

void Client::ServerUpdate()
{
	if (IsConnected())
	{
		while (!Incoming().empty())
		{
			auto msg = Incoming().pop_front().msg;

			switch (msg.header.type)
			{
			case MsgTypes::Server_Ping:
			{
				std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
				std::chrono::system_clock::time_point timeThen;
				msg >> timeThen;
				std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
			}
			break;
			case MsgTypes::Client_Accepted:
			{
				std::cout << "Server Accepted Connection\n";

				net::message<MsgTypes> msg;
				msg.header.type = MsgTypes::Client_RegisterWithServer;
				Send(msg);

				waitingToConnect = false;
			}
			break;
			case MsgTypes::Client_RegisterID: // id
			{
				msg >> m_ClientID;

				OnRegister();
			}
			break;
			case MsgTypes::Client_MakeOwner: // id
			{
				int64_t id;
				msg >> id;

				SpriteManager::MakeOwner(id);
			}
			break;
			case MsgTypes::Game_AssignID: // id + id
			{
				int64_t oldID, newID;
				msg >> newID >> oldID;

				SpriteManager::AssignID(oldID, newID);
			}
			break;
			case MsgTypes::Game_AddSprite: // desc + type + id
			{
				SpriteTypes type;
				int64_t id;
				msg >> id >> type;
				SpriteManager::AddSprite(id, type, msg.body);
			}
			break;
			case MsgTypes::Game_RemoveSprite: // id
			{
				int64_t id;
				msg >> id;

				SpriteManager::RemoveSpriteByID(id);
			}
			break;
			case MsgTypes::Game_UpdateSprite: // desc + id
			{
				int64_t id;
				msg >> id;
				SpriteManager::SyncSprite(id, msg.body);
			}
			break;
			case MsgTypes::Game_ForceUpdateSprite: // desc + id
			{
				int64_t id;
				msg >> id;
				SpriteManager::ForceSyncSprite(id, msg.body);
			}
			break;
			}
		}
	}
}

void Client::OnRegister()
{
	Player* m_Player = new Player();
	m_Player->m_Position = glm::vec3(-1.0f * m_ClientID, 0.0f, 0.0f);
	SpriteManager::AddSpriteWithID(m_ClientID, m_Player);
	Camera::SetFollowTarget(m_Player);

	Water* water = new Water();
	SpriteManager::AddSpriteLocally(water);

	Inventory* inventory = (new Inventory());
	UISpriteManager::AddSprite(inventory);

	if (m_ClientID == 1)
	{
		BlockGroup* blockGroup = new BlockGroup();

		blockGroup->m_Position = glm::vec3(12.0f, 0.0f, 0.0f);
		blockGroup->m_Rotation = glm::vec3(0.0f, 1.0f, 0.0f);

		// Temporary island generation
		for (int z = -10; z < 10; z++)
		{
			for (int x = -10; x < 10; x++)
			{
				if (glm::length(glm::vec3(x, 0, z)) <= 9.5f) blockGroup->SetBlock(glm::ivec3(x, 0, z), SAND);
				
			}
		}
		for (int z = -8; z < 8; z++)
		{
			for (int x = -8; x < 8; x++)
			{
				if (glm::length(glm::vec3(x, 0, z)) <= 7.5f) blockGroup->SetBlock(glm::ivec3(x, 0, z), GRASS);
			}
		}
		for (int z = -4; z < 4; z++)
		{
			for (int x = -4; x < 4; x++)
			{
				if (glm::length(glm::vec3(x, 0, z)) <= 3.5f) blockGroup->SetBlock(glm::ivec3(x, 1, z), GRASS);
			}
		}

		SpriteManager::AddSprite(blockGroup);
	}
}

void Client::PingServer()
{
    net::message<MsgTypes> msg;
    msg.header.type = MsgTypes::Server_Ping;

    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

    msg << timeNow;
    Send(msg);
}