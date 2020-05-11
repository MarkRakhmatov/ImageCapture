#pragma once
#include <stdint.h>

namespace Communication
{
	enum class EConnectionStatus : uint32_t
	{
	  FAIL,
	  SUCCESS,
	  SHUTDOWN,
	  DISCONNECT,

	};
}
