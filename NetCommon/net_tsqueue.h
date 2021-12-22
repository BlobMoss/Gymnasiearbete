#pragma once

#include "net_common.h"

namespace net
{
	template<typename I>
	class tsqueue
	{
	public:
		tsqueue() = default;
		tsqueue(const tsqueue<T>&) = delete;
		virtual ~tsqueue() { clear(); }

	public:
		// Returns item at front of queue
		const T& front()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.front;
		}

		// Returns item at back of queue
		const T& back()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.back;
		}

		// Add item to front of queue
		void push_front(const t& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_front(std::move(item));
		}

		// Add item to back of queue
		void push_back(const t& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_back(std::move(item));
		}

		// Returns true if queue has zero items
		bool empty()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.empty();
		}

		// Returns number of items in queue
		size_t count()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.size();
		}

		// Clears queue
		void clear()
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.clear();
		}

		// Removes and returns item at front of queue
		T pop_front()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(deqQueue.front());
			deqQueue.pop_front();
			return t;
		}

		// Removes and returns item at back of queue
		T pop_back()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(deqQueue.back());
			deqQueue.pop_back();
			return t;
		}

	protected:
		std::mutex muxQueue;
		std::deque<T> deqQueue;
	};
}