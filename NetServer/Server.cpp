#include <iostream>
#include <net.h>
#include <MessageTypes.h>

class Server : public net::server_interface<MsgTypes>
{
public:
	Server(uint16_t nPort) : net::server_interface<MsgTypes>(nPort)
	{

	}
	uint16_t m_ClientIDCounter;
	uint64_t m_SpriteIDCounter;
	std::unordered_map<int64_t, SpriteDescription> m_SpriteDescriptions;

protected:
	virtual bool OnClientConnect(std::shared_ptr<net::connection<MsgTypes>> client)
	{
		net::message<MsgTypes> msg;
		msg.header.id = MsgTypes::Client_Accepted;
		m_ClientIDCounter++;
		msg << m_ClientIDCounter;
		client->Send(msg);

		return true;
	}

	virtual void OnClientDisconnect(std::shared_ptr<net::connection<MsgTypes>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	virtual void OnMessage(std::shared_ptr<net::connection<MsgTypes>> client, net::message<MsgTypes>& msg)
	{
		switch (msg.header.id)
		{
		case MsgTypes::Server_Ping:
		{
			std::cout << "[" << client->GetID() << "] Server Ping\n";

			client->Send(msg);
		}
		break;
		case MsgTypes::Client_RegisterWithServer:
		{
			for (const auto& sprite : m_SpriteDescriptions)
			{
				net::message<MsgTypes> msg;
				msg.header.id = MsgTypes::Game_AddSprite;

				int64_t id = sprite.first;
				SpriteDescription desc = sprite.second;

				msg << id << desc;
				MessageClient(client, msg);
			}
		}
		break;
		case MsgTypes::Game_AddSprite:
		{
			SpriteDescription desc;
			int64_t oldID;
			msg >> desc >> oldID;

			m_SpriteIDCounter++;
			int64_t newID = m_SpriteIDCounter;

			msg << newID << desc;

			MessageAllClients(msg, client);

			net::message<MsgTypes> idMsg;
			idMsg.header.id = MsgTypes::Game_AssignID;
			idMsg << oldID << newID;

			MessageClient(client, idMsg);

			m_SpriteDescriptions.insert_or_assign(newID, desc);
		}
		break;
		case MsgTypes::Game_UpdateSprite:
		{
			SpriteDescription desc;
			int64_t id;

			msg >> desc >> id;

			m_SpriteDescriptions.insert_or_assign(id, desc);

			msg << id << desc;

			MessageAllClients(msg, client);
		}
		break;
		}
	}
};

int main()
{
	Server server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}

	return 0;
}