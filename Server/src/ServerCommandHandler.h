#pragma once
#include <vector>
#include <array>
#include <utility>
#include "Response.h"
#include "DescriptorHolder.h"
#include "Command.h"
#include "CommandHandlerData.h"
#include "IOnCommandSrv.h"

class ServerCommandHandler
{

public:
  ServerCommandHandler();
  EResponse Handle(DescriptorHolder& sock);
  static CommandHandlerData& GetData();
private:
  std::string mDeviceName{"/dev/video0"};
  static CommandHandlerData mData;
  std::array<std::unique_ptr<IOnCommandSrv>, static_cast<size_t>(ECommand::SIZE)>
  mCommandToHandler{};
};
