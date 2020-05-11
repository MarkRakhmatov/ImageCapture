#pragma once
#include "IOnCommandSrv.h"

namespace ServerSide
{
	class OnSetupSrv : public IOnCommandSrv
	{
	public:
		Communication::EConnectionStatus Handle(Communication::Socket& sock);
	};
}
