#pragma once

#include <global.hpp>
#include <memory>
#include <optional>
#include <string>

namespace AmqpClient { class Channel; }

namespace masstransit_cpp
{
	struct MASSTRANSIT_API ssl_config
	{
		std::string path_to_ca_cert;
		std::string path_to_client_key;
		std::string path_to_client_cert;
		bool verify_hostname{ false };

		friend bool MASSTRANSIT_API operator==(ssl_config const& lhs, ssl_config const& rhs);
		friend bool MASSTRANSIT_API operator!=(ssl_config const& lhs, ssl_config const& rhs);
	};

	struct MASSTRANSIT_API amqp_host
	{
		explicit amqp_host(std::string const& host);
		amqp_host(std::string const& host, int port, std::string const& virtual_host,
			std::string const& user, std::string const& password,
			std::optional<ssl_config> const& ssl);

		std::string host;
		std::string vhost{ "/" };
		int port{ 5672 };
		std::string user;
		std::string password;
		std::optional<ssl_config> ssl;

		std::string to_string() const;  // "amqp://username:password@host/"

		std::shared_ptr<AmqpClient::Channel> create_channel() const;
		
		static const std::string localhost;

		friend bool MASSTRANSIT_API operator == (amqp_host const& l, amqp_host const& r);
		friend bool MASSTRANSIT_API operator != (amqp_host const& l, amqp_host const& r);
	};
}
