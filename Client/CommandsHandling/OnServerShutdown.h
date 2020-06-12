#pragma once

#include "IOnCommand.h"
namespace ClientSide
{
	class OnServerShutdown : public IOnCommand
	{
	public:
		Communication::EConnectionStatus Handle(Communication::Socket& sock);
	private:
		Communication::EConnectionStatus GetResponse(Communication::Socket& sock);
	};
}
