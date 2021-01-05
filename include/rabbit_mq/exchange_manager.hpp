#pragma once

#include <global.hpp>
#include <memory>
#include <set>
#include <string>

namespace AmqpClient { class Channel; }

namespace masstransit_cpp
{
	namespace rabbit_mq
	{
		class MASSTRANSIT_API exchange_manager
		{
		public:
			exchange_manager(bool auto_delete);

			bool has_exchange(std::string const& name) const;

			void declare_exchange(std::string const& name, std::shared_ptr<AmqpClient::Channel> const& channel);
			
			std::set<std::string> const& all() const { return exchanges_; }

		protected:
			std::set<std::string> exchanges_;
			const bool auto_delete_{ false };
		};
	}
}
