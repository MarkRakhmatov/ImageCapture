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
	  EMPTY_COMMAND,
	  SETUP_CAMERA,
	  PROCESS_IMAGE,

	  SIZE
	};
}
