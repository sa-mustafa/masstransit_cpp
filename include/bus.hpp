#pragma once

#include <global.hpp>
#include <bus_control.hpp>
#include <i_publish_endpoint.hpp>

namespace masstransit_cpp
{
	class MASSTRANSIT_API bus : public bus_control, public i_publish_endpoint
	{
	public:
		~bus() override = default;
	};
}
