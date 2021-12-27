#include "Client.h"

Client::Client()
	: waitingToConnect(true)
{

}

void Client::ServerUpdate()
{
	if (IsConnected())
	{
		if (!Incoming().empty())
		{
			auto msg = Incoming().pop_front().msg;

			switch (msg.header.id)
			{
			case MsgTypes::Client_Accepted:
			{
				msg >> m_ClientID;
				waitingToConnect = false;
				OnConnect();
				std::cout << "Server Accepted Connection\n";
			}
			break;
			case MsgTypes::Server_Ping:
			{
				std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
				std::chrono::system_clock::time_point timeThen;
				msg >> timeThen;
				std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
			}
			break;
			case MsgTypes::Game_AddSprite:
			{
				SpriteDescription desc;
				int64_t id;
				msg >> desc >> id;

				SpriteManager::AddSprite(id, new Sprite(new Model("res/models/gem.obj", "res/textures/gem_texture.png", "res/shaders/lighting.shader")));
			}
			break;
			case MsgTypes::Game_AssignID:
			{
				int64_t oldID, newID;
				msg >> newID >> oldID;

				SpriteManager::AssignID(oldID, newID);
			}
			break;
			case MsgTypes::Game_UpdateSprite:
			{
				int64_t id;
				SpriteDescription desc;
				msg >> desc >> id;
				
				SpriteManager::SyncSprite(id, desc);
			}
			break;
			}
		}
	}

	if (Input::KeyHeld(KEY_A)) player->SetPosition(player->GetPosition() + glm::vec3(0.0f, 0.0f, 0.1f));
}

void Client::OnConnect()
{
	player = new Sprite(new Model("res/models/gem.obj", "res/textures/gem_texture.png", "res/shaders/lighting.shader"));
	SpriteManager::AddSprite(player);
}

void Client::PingServer()
{
    net::message<MsgTypes> msg;
    msg.header.id = MsgTypes::Server_Ping;

    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

    msg << timeNow;
    Send(msg);
}