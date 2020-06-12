#pragma once

#include "IOnCommandSrv.h"

namespace ServerSide
{
	class OnShutdownSrv : public IOnCommandSrv
	{
	public:
		Communication::EConnectionStatus Handle(Communication::Socket& sock);
	};
}
