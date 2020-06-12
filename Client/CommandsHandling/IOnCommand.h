#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Socket.h"
#include "Response.h"

namespace ClientSide
{
	class IOnCommand
	{
	public:
	  virtual ~IOnCommand(){};
	  virtual Communication::EConnectionStatus Handle(Communication::Socket& sock) = 0;
	};
}
