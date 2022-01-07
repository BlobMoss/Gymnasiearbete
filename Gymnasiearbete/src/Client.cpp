#include "Client.h"

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
			case MsgTypes::Client_RegisterID:
			{
				msg >> m_ClientID;

				OnRegister();
			}
			break;
			case MsgTypes::Client_MakeOwner:
			{
				int64_t id;
				msg >> id;

				SpriteManager::MakeOwner(id);
			}
			break;
			case MsgTypes::Game_AssignID:
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
			}
		}
	}
}

void Client::OnRegister()
{
	Player* m_Player = new Player();
	SpriteManager::AddSpriteWithID(m_ClientID, m_Player);
	Camera::SetFollowTarget(m_Player);

	Body* body = new Body();
	body->m_Position = glm::vec3(m_ClientID, 0.0f, 0.0f);
	body->m_Model = new Model("res/models/gem.obj", "res/textures/gem_texture.png", "res/shaders/lighting.shader");
	SpriteManager::AddSprite(body);

	if (m_ClientID == 1)
	{
		BlockGroup* blockGroup = new BlockGroup();

		blockGroup->SetBlock(glm::ivec3(34, 0, 31), GRASS);
		blockGroup->SetBlock(glm::ivec3(34, 0, 32), GRASS);
		blockGroup->SetBlock(glm::ivec3(34, 0, 33), GRASS);
		blockGroup->SetBlock(glm::ivec3(33, 0, 33), GRASS);
		blockGroup->SetBlock(glm::ivec3(32, 0, 33), SAND);
		blockGroup->SetBlock(glm::ivec3(33, 0, 32), SAND);
		blockGroup->SetBlock(glm::ivec3(33, 0, 31), SAND);
		blockGroup->SetBlock(glm::ivec3(32, 0, 32), SAND);
		blockGroup->SetBlock(glm::ivec3(31, 0, 32), SAND);
		blockGroup->SetBlock(glm::ivec3(32, 0, 31), GRASS);

		blockGroup->SetBlock(glm::ivec3(34, 1, 31), GRASS);
		blockGroup->SetBlock(glm::ivec3(34, 1, 32), GRASS);
		blockGroup->SetBlock(glm::ivec3(34, 1, 33), GRASS);

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