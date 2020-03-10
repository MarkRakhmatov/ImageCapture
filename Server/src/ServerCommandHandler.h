#pragma once
#include <vector>
#include <array>
#include <utility>
#include <chrono>
#include "Response.h"
#include "DescriptorHolder.h"
#include "Command.h"
#include "CommandHandlerData.h"
#include "IOnCommandSrv.h"
#include "Socket.h"

class ServerCommandHandler
{

public:
  ServerCommandHandler();
  EConnectionStatus Handle(Socket& sock);
  static CommandHandlerData& GetData();
private:
  static CommandHandlerData mData;
  std::array<std::unique_ptr<IOnCommandSrv>, static_cast<size_t>(ECommand::SIZE)>
  mCommandToHandler{};
};
