#include "Client.h"

#include "graphics/Water.h"

#include "gameplay/World.h"

#include "ui/Inventory.h"
#include "ui/Crafting.h"
#include "ui/HealthBar.h"
#include "ui/RespawnMenu.h"

uint32_t Client::m_ClientID = 0;

std::string Client::m_ThisIP;

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
	m_Player->m_Position = glm::vec3(-1.0f * m_ClientID, 0.0f, 25.0f);
	SpriteManager::AddSpriteWithID(m_ClientID, m_Player);
	Camera::SetFollowTarget(m_Player);

	Water* water = new Water();
	SpriteManager::AddSpriteLocally(water);

	new Inventory();
	new Crafting();
	new HealthBar();
	new RespawnMenu();

	new World(m_ClientID == 1);
}

void Client::PingServer()
{
    net::message<MsgTypes> msg;
    msg.header.type = MsgTypes::Server_Ping;

    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

    msg << timeNow;
    Send(msg);
}