#include "catch.hpp"

#include <masstransit_cpp/rabbit_mq/amqp_host.hpp>

namespace masstransit_cpp_tests
{
	using namespace masstransit_cpp;

	TEST_CASE("amqp_host_tests", "[amqp_host]")
	{
		SECTION("local_uri")
		{
			REQUIRE(amqp_host::localhost == "127.0.0.1");
		}

		SECTION("host_configurator::get_host")
		{
			amqp_host_configurator configurator("192.16.128.1");
			configurator.username("user");
			configurator.password("password");
			configurator.port(5672);

			amqp_host host("192.16.128.1", 5672, "user", "password");
			
			REQUIRE(configurator.get_host() == host);
		}

		SECTION("default_host_configurator::get_host")
		{
			amqp_host_configurator configurator("192.16.128.1");
			amqp_host host("192.16.128.1");
			amqp_host host_explicit("192.16.128.1", 5672, "", "");
			
			REQUIRE(configurator.get_host() == host);
			REQUIRE(host_explicit == host);
		}
	}
}