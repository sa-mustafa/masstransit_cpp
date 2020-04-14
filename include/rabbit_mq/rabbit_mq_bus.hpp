#pragma once

#include <global.hpp>
#include <bus.hpp>
#include <host_info.hpp>
#include <rabbit_mq/receive_endpoint.hpp>
#include <rabbit_mq/amqp_host.hpp>
#include <rabbit_mq/exchange_manager.hpp>
#include <rabbit_mq/message_publisher.hpp>

namespace masstransit_cpp
{
	namespace threads {
		class task_repeat;
		class thread_pool;
		class worker_thread;
	}

	class MASSTRANSIT_API rabbit_mq_bus : public bus, public std::enable_shared_from_this<rabbit_mq_bus>
	{
	public:
		~rabbit_mq_bus() override;

		void start() override;
		void wait() const override;
		void stop() override;

	protected:
		rabbit_mq_bus(amqp_host const& target_host,
			host_info const& client_info,
			std::shared_ptr<rabbit_mq::exchange_manager> const& exchange_manager,
			std::vector<rabbit_mq::receive_endpoint::factory> const& receivers_factories);
		
		std::future<bool> publish(consume_context_info const& message, std::string const& exchange) const override;

		void fill(consume_context_info & message, std::string const& exchange) const override;

	friend class rabbit_mq_configurator;

	private:
		std::shared_ptr<AmqpClient::Channel> queue_channel_;
		std::shared_ptr<rabbit_mq::exchange_manager> exchange_manager_;
		std::vector<std::shared_ptr<rabbit_mq::receive_endpoint>> receivers_;
		const std::vector<rabbit_mq::receive_endpoint::factory> receivers_factories_;
		const amqp_host target_host_;
		const host_info host_info_;
		const message_publisher message_publisher_;

		std::shared_ptr<threads::thread_pool> receive_workers_;
		std::unique_ptr<threads::worker_thread> publish_worker_;
	};
}
