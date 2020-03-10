#pragma once
#include <vector>
#include <array>
#include <utility>
#include <chrono>
#include "Response.h"
#include "Socket.h"
#include "Command.h"
#include "IOnCommand.h"

class CommandHandler
{
public:
  CommandHandler();
  void Handle(Socket& sock, ECommand command);
private:
  std::array<std::unique_ptr<IOnCommand>,
              static_cast<size_t>(ECommand::SIZE)> mCommandToHandler{};
};
