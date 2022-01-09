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
			}
		}
	}
}

void Client::OnRegister()
{
	Player* m_Player = new Player();
	SpriteManager::AddSpriteWithID(m_ClientID, m_Player);
	Camera::SetFollowTarget(m_Player);

	//Body* body = new Body();
	//body->m_Position = glm::vec3(2 + m_ClientID * 2, 0.0f, 0.0f);
	//body->m_Model = new Model("res/models/gem.obj", "res/textures/gem_texture.png", "res/shaders/lighting.shader");
	//SpriteManager::AddSprite(body);

	if (m_ClientID == 1)
	{
		BlockGroup* blockGroup = new BlockGroup();

		blockGroup->m_Position = glm::vec3(10.0f, 0.0f, 5.0f);
		//blockGroup->m_Rotation = glm::vec3(0.0f, 1.0f, 0.0f);

		blockGroup->SetBlock(glm::ivec3(2, 0, -1), GRASS);
		blockGroup->SetBlock(glm::ivec3(2, 0, 0), GRASS);
		blockGroup->SetBlock(glm::ivec3(2, 0, 1), GRASS);
		blockGroup->SetBlock(glm::ivec3(1, 0, 1), GRASS);
		blockGroup->SetBlock(glm::ivec3(0, 0, 1), SAND);
		blockGroup->SetBlock(glm::ivec3(1, 0, 0), SAND);
		blockGroup->SetBlock(glm::ivec3(1, 0, -1), SAND);
		blockGroup->SetBlock(glm::ivec3(0, 0, 0), SAND);
		blockGroup->SetBlock(glm::ivec3(-1, 0, 0), SAND);
		blockGroup->SetBlock(glm::ivec3(0, 0, -1), GRASS);
		blockGroup->SetBlock(glm::ivec3(2, 0, 10), GRASS);

		blockGroup->SetBlock(glm::ivec3(2, 1, -1), GRASS);
		blockGroup->SetBlock(glm::ivec3(2, 1, 0), GRASS);
		blockGroup->SetBlock(glm::ivec3(2, 1, 1), GRASS);
		blockGroup->SetBlock(glm::ivec3(2, 1, 10), GRASS);

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