#pragma once
#include <vector>
#include <array>
#include <utility>
#include "Response.h"
#include "DescriptorHolder.h"
#include "Command.h"
#include "IOnCommand.h"

class CommandHandler
{
public:
  CommandHandler();
  EResponse Handle(DescriptorHolder& sock, ECommand command);
private:
  std::array<std::unique_ptr<IOnCommand>, static_cast<size_t>(ECommand::SIZE)>
  mCommandToHandler{};
};
