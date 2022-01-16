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
	m_Player->m_Position = glm::vec3(0.0f, 0.0f, 5.0f);
	SpriteManager::AddSpriteWithID(m_ClientID, m_Player);
	Camera::SetFollowTarget(m_Player);

	Body* body = new Body();
	body->m_Position = glm::vec3(30, 0.0f, 0.0f);
	body->m_Model = new Model("res/models/gem.obj", "res/textures/gem_texture.png", "res/shaders/lighting.shader");
	SpriteManager::AddSprite(body);

	if (m_ClientID == 1)
	{
		BlockGroup* blockGroup = new BlockGroup();

		blockGroup->m_Position = glm::vec3(0.0f, 0.0f, 5.0f);

		blockGroup->SetBlock(glm::ivec3(-1, 0, -5), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 1, -5), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, -5), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, -5), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 1, -5), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-2, 0, -4), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 1, -4), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 0, -4), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, -4), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, -4), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 0, -4), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 1, -4), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-2, 0, -3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 1, -3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 0, -3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, -3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, -3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 0, -3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 1, -3), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-3, 0, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-3, 1, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 0, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 1, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 0, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 1, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 1, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 0, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 1, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 0, -2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 1, -2), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-3, 0, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-3, 1, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 0, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 0, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 0, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 0, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 1, -1), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-3, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-3, 1, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 1, 0), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-3, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-3, 1, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 1, 1), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-3, 0, 2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-3, 1, 2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 0, 2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 0, 2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, 2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, 2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 0, 2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 0, 2), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 1, 2), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-3, 0, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 0, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 0, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 0, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 0, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-3, 1, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-2, 1, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 1, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 1, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 1, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(2, 1, 3), PLANKS);
		blockGroup->SetBlock(glm::ivec3(3, 1, 3), PLANKS);

		SpriteManager::AddSprite(blockGroup);
	}

	if (m_ClientID == 2)
	{
		BlockGroup* blockGroup = new BlockGroup();

		blockGroup->m_Position = glm::vec3(0.0f, 0.0f, -5.0f);

		blockGroup->SetBlock(glm::ivec3(0, 0, -2), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-1, 0, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, -1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, -1), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-1, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 1, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, 0), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 1, 0), PLANKS);

		blockGroup->SetBlock(glm::ivec3(-1, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 0, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(-1, 1, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(0, 1, 1), PLANKS);
		blockGroup->SetBlock(glm::ivec3(1, 1, 1), PLANKS);

		SpriteManager::AddSprite(blockGroup);
	}

	if (m_ClientID == 1)
	{
		BlockGroup* blockGroup = new BlockGroup();

		blockGroup->m_Position = glm::vec3(25.0f, 0.0f, 0.0f);

		// Temporary island generation
		for (int z = -10; z < 10; z++)
		{
			for (int x = -10; x < 10; x++)
			{
				if (glm::length(glm::vec3(x, 0, z)) <= 10) blockGroup->SetBlock(glm::ivec3(x, 0, z), SAND);
				
			}
		}
		for (int z = -8; z < 8; z++)
		{
			for (int x = -8; x < 8; x++)
			{
				if (glm::length(glm::vec3(x, 0, z)) <= 8) blockGroup->SetBlock(glm::ivec3(x, 0, z), GRASS);
			}
		}
		for (int z = -4; z < 4; z++)
		{
			for (int x = -4; x < 4; x++)
			{
				if (glm::length(glm::vec3(x, 0, z)) <= 4) blockGroup->SetBlock(glm::ivec3(x, 1, z), GRASS);
			}
		}


		blockGroup->m_Static = true;

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