#pragma once

#include <utils/uuid.hpp>
#include <utils/json.hpp>

namespace uuids
{
	void to_json(nlohmann::json& j, uuids::uuid const& p);
	void from_json(nlohmann::json const& j, uuids::uuid& p);
}
