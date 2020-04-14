#pragma once

#include <threads/worker.hpp>

#include <list>

namespace masstransit_cpp
{
	namespace threads
	{
		class MASSTRANSIT_API thread_pool : public tasks_queue
		{
		public:
			explicit thread_pool(size_t pool_size, bool spreadOnCores = false);
			virtual ~thread_pool();

			void wait() const;

		private:
			std::list<worker> workers_;
		};
	}
}