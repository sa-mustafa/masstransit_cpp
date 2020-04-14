#pragma once

#include <global.hpp>
#include <consume_context_info.hpp>

namespace AmqpClient { class Channel; }

namespace masstransit_cpp
{
	class MASSTRANSIT_API message_publisher
	{
	public:
		bool publish(consume_context_info const& context, std::shared_ptr<AmqpClient::Channel> const& channel, std::string const& exchange) const;
	};
}
