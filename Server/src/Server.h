#pragma once
#include <Command.h>
#include <netinet/in.h>
#include "DescriptorHolder.h"
#include "ServerCommandHandler.h"
#include "Socket.h"

namespace ServerSide
{
	class Server
	{
	public:
	  Server();
	  Communication::EConnectionStatus Listen();
	private:
	  Communication::EConnectionStatus HandleCommand();
	  DescriptorHolder mListener;
	  sockaddr_in mAddr{};
	  ServerCommandHandler mCommandHandler;
	};
}
