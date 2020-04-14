#pragma once
#include <threads/tasks_queue.hpp>

namespace masstransit_cpp
{
	namespace threads
	{
		class MASSTRANSIT_API worker
		{
		public:
			explicit worker(tasks_queue & queue);
			~worker();

			void attach() const;

		protected:
			std::future<void> future_;
		};
	}
}
