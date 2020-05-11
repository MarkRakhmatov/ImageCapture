#pragma once
#include "DescriptorHolder.h"
#include "Socket.h"
#include "Response.h"

namespace ServerSide
{
	class IOnCommandSrv
	{
	public:
	  virtual ~IOnCommandSrv(){};
	  virtual Communication::EConnectionStatus Handle(Communication::Socket& sock) = 0;
	};
}
