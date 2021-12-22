#pragma once

#include "net_common.h"

namespace net
{
	template <typename T>
	struct message_header
	{
		T id{};
		uint32_t size = 0;
	};

	template <typename T>
	struct message
	{
		message_header<T> header{};
		std::vector<uint8_t> body;

		// Returns size of entire message in bytes, header + body
		size_t size() const
		{
			return sizeof(message_header<T>) + body.size();
		}

		// Overide of << useful when printing to console
		friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
		{
			os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
			return os;
		}

		// Pushes data into message buffer
		template<typename DataType>
		friend message<T>& operator << (message<T>& msg, const DataType& data)
		{
			// Make sure data is easily serializable. For example is not a class with static members
			static_assert(std::is_standard_layout<DataType>::value, "Data too complex to be pushed into byte vector.");

			size_t i = msg.body.size();

			// Resize vector to fit new data
			msg.body.resize(msg.body.size() + sizeof(DataType));

			// Copy data into newly allocated vector space
			std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

			// Set size of new vector in message header
			msg.header.size = msg.size();

			// Returning message allows operator to be chained
			return msg;
		}

		// Retrieves data from message buffer
		template<typename DataType>
		friend message<T>& operator >> (message<T>& msg, DataType& data)
		{
			// Make sure data is easily serializable. For example is not a class with static members
			static_assert(std::is_standard_layout<DataType>::value, "Data too complex to be pushed into byte vector.");

			size_t i = msg.body.size() - sizeof(DataType);

			// Copy data from message into target variable
			std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

			// Resize vector to remove read data
			msg.body.resize(i);

			// Set size of new vector in message header
			msg.header.size = msg.size();

			// Returning message allows operator to be chained
			return msg;
		}
	};

	// Forward declare connection object
	template <typename T>
	class connection;

	template <typename T>
	struct owned_message
	{
		std::shared_ptr<connection<T>> remote = nullptr;
		message<T> msg;

		// Overide of << useful when printing to console
		friend std::ostream& operator << (std::ostream& os, const owned_message<T>& msg)
		{
			os << msg.msg;
			return os;
		}
	};
}