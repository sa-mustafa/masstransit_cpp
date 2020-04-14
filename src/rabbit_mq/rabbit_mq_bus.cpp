#include "rabbit_mq/rabbit_mq_bus.hpp"
#include "rabbit_mq/exchange_manager.hpp"
#include "rabbit_mq/receive_endpoint.hpp"
#include <threads/task_repeat.hpp>
#include <threads/thread_pool.hpp>
#include <threads/worker_thread.hpp>

#include <SimpleAmqpClient/Channel.h>

namespace masstransit_cpp
{
	rabbit_mq_bus::rabbit_mq_bus(amqp_host const& target_host, host_info const& client_info,
		std::shared_ptr<rabbit_mq::exchange_manager> const& exchange_manager,
		std::vector<rabbit_mq::receive_endpoint::factory> const& receivers_factories)
		: exchange_manager_(exchange_manager)
		, receivers_factories_(receivers_factories)
		, target_host_(target_host)
		, host_info_(client_info)
		, message_publisher_{}
	{
		queue_channel_ = target_host_.create_channel();
	}

	rabbit_mq_bus::~rabbit_mq_bus()
	{
	}

	void rabbit_mq_bus::start()
	{
		const auto this_ptr = shared_from_this();
		for (auto const& b : receivers_factories_)
		{
			auto receiver = b(this_ptr, host_info_);
			receiver->bind_queues(exchange_manager_);
			receivers_.push_back(receiver);
		}

		receive_workers_ = std::make_shared<threads::thread_pool>(std::thread::hardware_concurrency());
		publish_worker_ = std::make_unique<threads::worker_thread>();
	}

	void rabbit_mq_bus::wait() const
	{
		auto workers = receive_workers_;
		while (!workers->is_stopped())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			for (auto & q : receivers_)
			{
				// You may process the message on the current thread
				//r->try_consume();
				// or assign it to the thread pool.
				workers->enqueue([](std::shared_ptr<rabbit_mq::receive_endpoint> const& endpoint)
				{
					return endpoint->try_consume();
				}, q);
			}
		}
		workers = nullptr;
	}
	
	void rabbit_mq_bus::stop()
	{
		receive_workers_->stop();
		receive_workers_ = nullptr;
		publish_worker_ = nullptr;
		receivers_.clear();
	}

	std::future<bool> rabbit_mq_bus::publish(consume_context_info const& m, std::string const& e) const
	{
		return publish_worker_->enqueue([this](consume_context_info const& message, std::string const& exchange)
		{
			exchange_manager_->declare_exchange(exchange, queue_channel_);
			return message_publisher_.publish(message, queue_channel_, exchange);
		}, m, e);
	}

	void rabbit_mq_bus::fill(consume_context_info & message, std::string const& exchange) const
	{
		message.send_host = host_info_;
	}

}
