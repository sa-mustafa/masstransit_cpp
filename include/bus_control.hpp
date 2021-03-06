#pragma once

#include <global.hpp>

namespace masstransit_cpp
{
	class MASSTRANSIT_API bus_control
	{
	public:
		virtual ~bus_control() = default;

		virtual void start() = 0;
		virtual void wait() const = 0;
		virtual void stop() = 0;
	};
}
