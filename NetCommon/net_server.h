#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace net
{
	template<typename T>
	class server_interface
	{
	public:
		server_interface(uint16_t port
			: m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
		{

		}

		virtual ~server_interface()
		{
			Stop();
		}

		bool Start()
		{
			try
			{
				WaitForClientConnection();

				m_threadContext = std::thread([this]() { m_asioContext.run(); });
			}
			catch (std::exception& e)
			{
				// Something prohibited the server from listening
				std::cerr << "[SERVER] Exception: " << e.what() << "\n";
				return false;
			}

			std::cout << "[SERVER] Started!\n";
			return true;
		}

		void Stop()
		{
			// Request asio context to stop
			m_asioContext.stop();

			// Clean up context thread
			if (m_threadContext.joinable()) m_threadContext.join();

			std::cout << "[SERVER] Stopped!\n";
		}

		// ASYNC - Tell asio to wait for connection
		void WaitForClientConnection()
		{
			m_asioAcceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket)
				{
					if (!ec)
					{
						std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

						std::shared_ptr<connection<T>> newconn =
							std::make_shared<connection<T>>(connection<T>::owner::server,
								m_asioContext, std::move(socket), m_qMessagesIn);

						// Deny connection if false
						if (OnClientConnect(newconn))
						{
							// Add connection to container
							m_deqConnections.push_back(std::move(newconn));

							m_deqConnections.back()->ConnectToClient(nIDCounter++);

							std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved\n";
						}
						else
						{
							std::cout << "[-----] Connection Denied\n";
						}
					}
					else
					{
						// Error occured during acceptance
						std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
					}

					// Give context more work so it does not run out and close
					WaitForClientConnection();
				});
		}

		// Send message to specific client
		void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg)
		{
			// Check if client is connected
			if (client && client->IsConnected())
			{
				client->Send(msg);
			}
			else
			{
				// Client could not be contacted, must have disconnected
				OnClientDisconnect(client);
				client.reset();
				m_deqConnections.erase(
					std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
			}
		}

		// Send message to all clients
		void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr)
		{
			bool bInvalidClientExists = false;

			for (auto& client : m_deqConnections)
			{
				// Check if client is connected
				if (client && client->IsConnected())
				{
					if (client != pIgnoreClient)
						client->Send(msg);
				}
				else
				{
					// Client could not be contacted, must have disconnected
					OnClientDisconnect(client);
					client.reset();
					bInvalidClientExists = true;
				}
			}

			if (bInvalidClientExists)
				m_deqConnections.erase(
					std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
		}

		void Update(size_t nMaxMessages = -1)
		{
			size_t nMessageCount = 0;
			while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
			{
				// Grab front message
				auto msg = m_qMessagesIn.pop_front();

				// Pass to message handler
				OnMessage(msg.remote, msg.msg);

				nMessageCount++;
			}
		}

	protected:
		// Called when client connects
		virtual bool OnClientConnect(std::shared_ptr<connection<T>> client)
		{
			return false;
		}

		// Called when client disconnects
		virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
		{

		}

		// Called when message arrives
		virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
		{

		}

	protected:
		// Thread-safe queue for incoming messages
		tsqueue<owned_message<T>> m_qMessagesIn;

		// Container of validated connections
		std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

		asio::io_context m_asioContext;
		std::thread m_threadContext;

		asio::ip::tcp::acceptor m_asioAcceptor;

		// Every client has a unique identifier
		uint32_t nIDCounter = 10000;
	};
}