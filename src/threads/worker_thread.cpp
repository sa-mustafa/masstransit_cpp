#include "threads/worker_thread.hpp"

namespace masstransit_cpp
{
	namespace threads
	{
		worker_thread::worker_thread(uint32_t core) : worker_(*this, core)
		{
		}

		worker_thread::~worker_thread()
		{
			stop();
		}

		void worker_thread::wait() const
		{
			worker_.wait();
		}
	}
}
