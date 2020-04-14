#include "threads/thread_pool.hpp"

namespace masstransit_cpp
{
	namespace threads
	{
		thread_pool::thread_pool(size_t pool_size, bool spreadOnCores)
		{
			uint32_t total_cores_ = std::max<uint32_t>(std::thread::hardware_concurrency(), 1);
			for (size_t i = 0; i < pool_size; ++i)
			{
				uint32_t core = (spreadOnCores ? i % total_cores_ : UINT32_MAX);
				workers_.emplace_back(*this, core);
			}
		}

		thread_pool::~thread_pool()
		{
			stop();
		}

		void thread_pool::wait() const
		{
			for (auto & worker_ : workers_)
			{
				worker_.wait();
			}
		}

	}
}
