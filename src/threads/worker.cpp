#include "threads/worker.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace masstransit_cpp
{
	namespace threads
	{
		worker::worker(tasks_queue & queue, uint32_t core)
			: future_(std::async(std::launch::async, [&queue, core]()
		{

			bool error = false;
			if (core != UINT32_MAX)
			{
#ifdef _WIN32
				error = !SetThreadAffinityMask(GetCurrentThread(), 1 << core);
#else
				cpu_set_t cpuset;
				CPU_ZERO(&cpuset);
				CPU_SET(core, &cpuset);
				error = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
#endif
			}

			while (true)
			{
				std::function<void()> task;

				{
					std::unique_lock<std::mutex> lock(queue.queue_mutex_);
					queue.condition_.wait(lock, [&queue]
					{
						return queue.stop_ || !queue.tasks_.empty();
					});

					if (queue.stop_ && queue.tasks_.empty())
						return;

					task = std::move(queue.tasks_.front());
					queue.tasks_.pop();
				}

				task();
			}
		}))
		{
		}

		worker::~worker()
		{
			wait();
		}

		void worker::wait() const
		{
			if (future_.valid()) future_.wait();
		}
	}
}
