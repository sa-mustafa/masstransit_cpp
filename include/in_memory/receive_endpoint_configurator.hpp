#pragma once

#include <global.hpp>
#include <message_consumer.hpp>
#include <i_receive_endpoint_configurator.hpp>
#include <in_memory/receive_endpoint.hpp>

namespace masstransit_cpp
{
	namespace in_memory
	{
		class receive_endpoint;

		class MASSTRANSIT_API receive_endpoint_configurator : public i_receive_endpoint_configurator
		{
		public:
			receive_endpoint_configurator(std::string const& queue_name);
			~receive_endpoint_configurator() override;

			receive_endpoint_configurator & transport_concurrency_limit(size_t limit);

			receive_endpoint::factory get_factory() const;
		
		private:
			size_t transport_concurrency_limit_{ 1 };

			static std::shared_ptr<receive_endpoint> build(receive_endpoint_configurator configuration, std::shared_ptr<i_publish_endpoint> const& publish_endpoint);
		};
	}
}
