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

class CommandHandler
{
public:
  CommandHandler();
  EConnectionStatus Handle(Socket& sock, ECommand command);
private:
  ECommand GetCommand();
private:
  std::array<std::unique_ptr<IOnCommand>,
              static_cast<size_t>(ECommand::SIZE)> mCommandToHandler{};
};
