#pragma once
#include "Response.h"
#include "DescriptorHolder.h"
#include "Command.h"
#include "IOnCommandSrv.h"
#include "Socket.h"

#include <memory>
#include <vector>
#include <array>
#include <utility>
#include <chrono>

namespace ServerSide
{
	class ServerCommandHandler
	{
	public:
	  ServerCommandHandler();
	  Communication::EConnectionStatus Handle(Communication::Socket& sock);
	private:
	  std::array<std::unique_ptr<IOnCommandSrv>, static_cast<size_t>(Communication::ECommand::SIZE)>
	  mCommandToHandler{};
	};
}
