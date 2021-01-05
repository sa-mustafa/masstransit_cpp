#include <utils/json_adapters.hpp>

void uuids::to_json(nlohmann::json& j, uuids::uuid const& p)
{
	j = to_string(p);
}

void uuids::from_json(nlohmann::json const& j, uuids::uuid& p)
{
	p = uuids::uuid::from_string(j.get<std::string>());
}
