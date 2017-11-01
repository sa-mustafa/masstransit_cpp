#include "masstransit_cpp/in_memory/in_memory_bus.hpp"

#include <boost/log/trivial.hpp>

namespace masstransit_cpp
{
	in_memory_bus::in_memory_bus(std::vector<in_memory::receive_endpoint::factory> const& receivers_factories)
		: receivers_factories_(receivers_factories)
	{
	}

	in_memory_bus::~in_memory_bus()
	{
	}

	void in_memory_bus::start()
	{
		for (auto const& factory : receivers_factories_)
		{
			receivers_.push_back(factory());
		}

		publish_worker_ = std::make_unique<threads::worker_thread>();
	}

	void in_memory_bus::stop()
	{
		publish_worker_ = nullptr;
		receivers_.clear();
	}

	std::future<bool> in_memory_bus::publish_impl(consume_context_info const& m, std::string const& t) const
	{
		return publish_worker_->enqueue([this](consume_context_info const& message, std::string const& type) -> bool {
			auto body = nlohmann::json(message).dump(2);
			try
			{
				BOOST_LOG_TRIVIAL(debug) << "bus publish message:\n" << body;

				for (auto const& receiver : receivers_)
					receiver->deliver(message);

				BOOST_LOG_TRIVIAL(debug) << "[DONE]";
				return true;
			}
			catch (std::exception & ex)
			{
				BOOST_LOG_TRIVIAL(error) << "rabbit_mq_bus::publish_impl\n\tException: " << ex.what();
				return false;
			}
			catch (...)
			{
				BOOST_LOG_TRIVIAL(error) << "rabbit_mq_bus::publish_impl\n\tException: unknown";
				return false;
			}
		}, m, t);
	}
}