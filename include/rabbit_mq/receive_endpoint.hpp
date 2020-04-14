#pragma once

#include <global.hpp>
#include <rabbit_mq/amqp_host.hpp>
#include <i_receive_endpoint.hpp>
#include <chrono>

namespace AmqpClient { class Channel; }

namespace masstransit_cpp
{
	namespace rabbit_mq
	{
		class exchange_manager;

		class MASSTRANSIT_API receive_endpoint : public i_receive_endpoint
		{
		public:
			using factory = std::function<std::shared_ptr<receive_endpoint>(std::shared_ptr<i_publish_endpoint> const&, host_info const&)>;

			receive_endpoint(std::shared_ptr<AmqpClient::Channel> const& channel, 
				std::string const& queue, host_info const& host,
				uint16_t prefetch_count, std::chrono::milliseconds const& timeout,
				consumers_map const& consumers_by_type,
				std::shared_ptr<i_publish_endpoint> const& publish_endpoint);

			bool try_consume() const;
			void bind_queues(std::shared_ptr<exchange_manager> const& exchange_manager);

			static std::string get_error_queue(std::string const& queue);

		private:
			const std::string queue_;
			const host_info host_;
			const uint16_t prefetch_count_;
			const int timeout_ms_;

			std::shared_ptr<AmqpClient::Channel> channel_;
			std::string tag_;

			static int get_ms(std::chrono::milliseconds const& timeout);

			void on_error(consume_context_info context, std::string const& consumer_type, 
				std::string const& message, std::exception const& ex) const;
		};
	}
}
