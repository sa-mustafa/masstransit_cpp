#pragma once

#include <threads/worker.hpp>

namespace masstransit_cpp
{
	namespace threads
	{
		class MASSTRANSIT_API worker_thread : public tasks_queue
		{
		public:
			worker_thread();
			virtual ~worker_thread();

			void wait() const;

		protected:
			worker worker_;
		};
	}
}
