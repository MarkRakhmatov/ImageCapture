#pragma once
#include "IOnCommand.h"

namespace ClientSide
{
	class OnCapture : public IOnCommand
	{
		Communication::EConnectionStatus Handle(Communication::Socket& sock);
	private:
		Communication::EConnectionStatus SendRequest(Communication::Socket& sock);
		Communication::EConnectionStatus GetResponse(Communication::Socket& sock);
	};
}
