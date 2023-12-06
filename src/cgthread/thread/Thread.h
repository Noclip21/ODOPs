#pragma once

#include <cstdint>
#include <functional>
#include <thread>
#include <mutex>


class Thread
{
	private:

		std::thread _thread;
		std::mutex  _dispatchMutex;
		std::mutex  _returnMutex;
		bool        _kill;
		std::function<void(void)> _callback;

		inline static void _loop(Thread*);

		inline Thread();
		inline ~Thread();

	
	public:

		inline static Thread& getThread(const uint64);
		inline static const uint64 numThreads();

		inline void setCallback(auto);
		inline void dispatch();
		inline void sync();
};
