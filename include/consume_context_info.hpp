#pragma once

#include <global.hpp>
#include <host_info.hpp>
#include <utils/json.hpp>
#include <utils/uuid.hpp>

#include <optional>

namespace masstransit_cpp
{
	struct MASSTRANSIT_API consume_context_info
	{
		uuids::uuid message_id;
		uuids::uuid conversation_id;
		std::optional<uuids::uuid> correlation_id;
		std::string source_address;
		std::string destination_address;

		std::vector<std::string> message_types; // example: "urn:message:MyNamespace.MySubNamespace:MessageClassName",

		nlohmann::json message;

		host_info send_host;
		std::map<std::string, std::string> headers;

		consume_context_info();

		template<typename MessageT>
		static consume_context_info create(MessageT const& message)
		{
			consume_context_info info;
			
			uuids::random_generator random;
			info.message_id = random();
			info.conversation_id = random();
			
			info.message_types = { "urn:message:" + MessageT::message_type() };
			info.message = nlohmann::json(message);
			return std::move(info);
		}

		friend bool MASSTRANSIT_API operator==(consume_context_info const& lhv, consume_context_info const& rhv);

		template<typename MessageT>
		consume_context_info create_response(MessageT const& message)
		{
			consume_context_info info;

			uuids::random_generator random;
			info.message_id = random();
			info.conversation_id = conversation_id;
			info.correlation_id = message_id;

			info.message_types = { "urn:message:" + MessageT::message_type() };
			info.message = nlohmann::json(message);
			return std::move(info);
		}
	};

	void MASSTRANSIT_API to_json(nlohmann::json& j, consume_context_info const& p);
	void MASSTRANSIT_API from_json(nlohmann::json const& j, consume_context_info & p);
}
