#include "rabbit_mq/amqp_host_configurator.hpp"

namespace masstransit_cpp
{	
	amqp_host_configurator::amqp_host_configurator(std::string const& host, std::string const& virtual_host)
		: host_(host)
		, vhost_(virtual_host)
	{
	}

	amqp_host_configurator& amqp_host_configurator::port(int port)
	{
		port_ = port;
		return *this;
	}

	amqp_host_configurator& amqp_host_configurator::username(std::string const& username)
	{
		username_ = username;
		return *this;
	}

	amqp_host_configurator& amqp_host_configurator::password(std::string const& password)
	{
		password_ = password;
		return *this;
	}

	amqp_host_configurator& amqp_host_configurator::use_ssl(std::function<void(ssl_config&)> const& configure)
	{
		if(ssl_ == std::nullopt)
			ssl_ = ssl_config();

		configure(ssl_.value());
		return *this;
	}

	amqp_host amqp_host_configurator::get_host() const
	{
		return amqp_host{ host_, port_, vhost_, username_, password_, ssl_ };
	}
}
