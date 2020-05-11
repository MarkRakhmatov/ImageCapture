#pragma once
#include "Response.h"
#include "Socket.h"
#include "Command.h"
#include "IOnCommand.h"

#include <vector>
#include <array>
#include <utility>
#include <chrono>
#include <unordered_map>

namespace ClientSide
{
	class CommandHandler
	{
	public:
	  CommandHandler();
	  Communication::EConnectionStatus Handle(Communication::Socket& sock, Communication::ECommand command);
	private:
	  Communication::ECommand GetCommand();
	private:
	  std::array<std::unique_ptr<IOnCommand>,
				  static_cast<size_t>(Communication::ECommand::SIZE)> mCommandToHandler{};
	};
}
