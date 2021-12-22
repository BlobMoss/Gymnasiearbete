#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace net
{
	template <typename T>
	class client_interface
	{
		client_interface() : m_socket(m_context)
		{
			// Initialize socket with the io context
		}

		virtual ~client_interface()
		{
			// Disconnect if client is destroyed
			Disconnect();
		}

	public:
		// Connect to server with hostname/ip-adress and port
		bool Connect(const std::string& host, const uint16_t port)
		{
			try
			{
				// Create connection object
				m_connection = std::make_unique<connection<T>>();

				// Resolve hostname/ip-adress
				asio::ip::tcp::resolver resolver(m_context);
				m_endpoints = resolver.resolve(host, std::to_string(port));

				// Tell connection object to connect to server
				m_connection->ConnectToServer(m_endpoints);

				// Start context thread
				thrContext = std::thread([this]() { m_context.run(); });
			}
			catch (std::exception& e)
			{
				std::cerr << "Client Exception: " << e.what() << "\n";
				return false;
			}

			return false;
		}

		// Disconnect from server
		void Disconnect()
		{
			if (IsConnected())
			{
				// Disconnect if connected
				m_connection->Disconnect();
			}

			// Stop asio context and its thread either way
			m_context.stop();

			if (thrContext.joinable())
				thrContext.join();

			m_connection.release();
		}

		// Check if client is connected to a server
		bool IsConnected()
		{
			if (m_connection)
				return m_connection->IsConntected();
			else
				return false;
		}

		// Retrieve queue from server
		tsqueue<owned_message<T>>& Incoming()
		{
			return m_qMessagesIn;
		}

	protected:
		// Asio context handles data transfer
		asio::io_context m_context;
		// Asio context needs its own thread to execute work commands
		std::thread thrContext;
		// Hardware socket connected to server
		asio::ip::tcp::socket m_socket;
		// Instance of connection object which handles data transfer
		std::unique_ptr<connection<T>> m_connection;

	private:
		// Thread-safe queue of incoming messages from server
		tsqueue<owned_message<T>> m_qMessagesIn;
	};
}