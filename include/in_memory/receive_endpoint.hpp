#pragma once

#include <global.hpp>
#include <i_receive_endpoint.hpp>
#include <message_consumer.hpp>
#include <threads/worker_thread.hpp>

namespace masstransit_cpp
{
	namespace in_memory
	{
		class exchange_manager;
		
		class MASSTRANSIT_API receive_endpoint : public i_receive_endpoint
		{
		public:
			using factory = std::function<std::shared_ptr<receive_endpoint>(std::shared_ptr<i_publish_endpoint> const& publish_endpoint)>;
			
			explicit receive_endpoint(std::string const& queue, 
				consumers_map const& consumers_by_type, 
				std::shared_ptr<i_publish_endpoint> const& publish_endpoint);

			template<class MessageT>
			void deliver(consume_context<MessageT> const& context)
			{
				consumer_worker_.enqueue([this](consume_context<MessageT> const& message) {
					auto message_context = message;
					auto consumer = std::static_pointer_cast<message_consumer<MessageT>>(find_consumer(message_context.message_types));
					if (consumer == nullptr)
						return;

					auto body = nlohmann::json(message_context.message).dump(2);
					try
					{
						//BOOST_LOG_TRIVIAL(debug) << "bus consumed message:\n" << body;
						
						consumer->consume(message_context);

						//BOOST_LOG_TRIVIAL(debug) << "[DONE]";
					}
					catch (std::exception & ex)
					{
						std::cout /*BOOST_LOG_TRIVIAL(error)*/ << "when bus consumer[" << consumer->message_type() << "] try handle message:\n" << body << "\n\tException: " << ex.what();
					}
					catch (...)
					{
						std::cout /*BOOST_LOG_TRIVIAL(error)*/ << "when bus consumer[" << consumer->message_type() << "] try handle message:\n" << body << "\n\tException: unknown";
					}
				}, context);
			}

			void deliver(consume_context_info const& context);

		private:
			const std::string queue_;
			threads::worker_thread consumer_worker_;
		};
	}
}
