#include <iostream>
#include <net.h>
#include <MessageTypes.h>

class Server : public net::server_interface<MsgTypes>
{
public:
	Server(uint16_t nPort) : net::server_interface<MsgTypes>(nPort),
		m_SpriteIDCounter(128)
	{

	}
	uint64_t m_SpriteIDCounter;
	std::unordered_map<int64_t, std::vector<uint8_t>> m_SpriteDescriptions;
	// desc + type
	std::vector<int64_t> m_GarbageIDs;

protected:
	bool OnClientConnect(std::shared_ptr<net::connection<MsgTypes>> client) override
	{
		return true;
	}

	void OnClientValidated(std::shared_ptr<net::connection<MsgTypes>> client) override
	{
		net::message<MsgTypes> msg;
		msg.header.type = MsgTypes::Client_Accepted;
		client->Send(msg);
	}

	void OnClientDisconnect(std::shared_ptr<net::connection<MsgTypes>> client) override
	{
		if (client)
		{
			m_SpriteDescriptions.erase(client->GetID());
			m_GarbageIDs.push_back(client->GetID());
		}
	}

	void OnMessage(std::shared_ptr<net::connection<MsgTypes>> client, net::message<MsgTypes>& msg) override
	{
		if (!m_GarbageIDs.empty())
		{
			for (auto id : m_GarbageIDs)
			{
				net::message<MsgTypes> m;
				m.header.type = MsgTypes::Game_RemoveSprite;
				m << id;
				std::cout << "Removing " << id << "\n";
				MessageAllClients(m);
				// id
			}
			m_GarbageIDs.clear();
		}
		//std::cout << "Server: " << msg << std::endl;
		switch (msg.header.type)
		{
		case MsgTypes::Server_Ping:
		{
			std::cout << "[" << client->GetID() << "] Server Ping\n";

			client->Send(msg);
		}
		break;
		case MsgTypes::Client_RegisterWithServer:
		{
			net::message<MsgTypes> msgSendID;
			msgSendID.header.type = MsgTypes::Client_RegisterID;
			msgSendID << client->GetID();
			MessageClient(client, msgSendID);

			for (const auto& sprite : m_SpriteDescriptions)
			{
				net::message<MsgTypes> msg;
				msg.header.type = MsgTypes::Game_AddSprite;

				msg.body = sprite.second;
				msg.header.size = msg.size();
				msg << sprite.first;

				std::cout << msg << std::endl;

				MessageClient(client, msg);
				// desc + type + id
			}
		}
		break;
		case MsgTypes::Game_AddSprite: // desc + type + id
		{
			int64_t oldID;
			msg >> oldID;

			int64_t newID = m_SpriteIDCounter;
			m_SpriteIDCounter++;
			msg << newID;

			MessageAllClients(msg, client);
			// desc + type + id

			int64_t id;
			msg >> id;
			m_SpriteDescriptions.insert_or_assign(id, msg.body);
			// desc + type

			net::message<MsgTypes> idMsg;
			idMsg.header.type = MsgTypes::Game_AssignID;
			idMsg << oldID << newID;

			MessageClient(client, idMsg);
			// id + id
		}
		break;
		case MsgTypes::Game_AddSpriteWithID: // desc + type + id
		{
			msg.header.type = MsgTypes::Game_AddSprite;

			MessageAllClients(msg, client);
			// desc + type + id

			int64_t id;
			msg >> id;
			m_SpriteDescriptions.insert_or_assign(id, msg.body);
			// desc + type
		}
		break;
		case MsgTypes::Game_UpdateSprite: // desc + type + id
		{
			int64_t id;
			msg >> id;
			m_SpriteDescriptions.insert_or_assign(id, msg.body);
			MsgTypes type;
			bool remove;
			msg >> type >> remove;
			if (remove) m_SpriteDescriptions.erase(id);
			msg << remove << id;

			MessageAllClients(msg, client);
			//desc + id
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