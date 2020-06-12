#pragma once
#include <stdint.h>

namespace Communication
{
	enum class ECommand : uint32_t
	{
	  PROCESS_IMAGE,
	  SERVER_SHUTDOWN,
	  EXIT,

	  SIZE
	};

	enum class EProcessImage : uint32_t
	{
	  SETUP_CAMERA,
	  CAPTURE,

	  SIZE
	};
}
