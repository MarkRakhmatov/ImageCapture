#pragma once
#include "IOnCommand.h"

namespace ClientSide
{
	class OnExit : public IOnCommand
	{
	public:
		Communication::EConnectionStatus Handle(Communication::Socket& sock);
	};
}
