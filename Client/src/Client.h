#pragma once
#include "CommandHandler.h"
#include "Command.h"
#include "Response.h"
#include "Socket.h"

#include <string>
#include <netinet/in.h>

class Client
{
public:
  Client(const std::string& serverIp = "192.168.7.2");
  EConnectionStatus Communicate();
  ECommand GetCommand();
  void EnsureConnection();
  bool IsConnected();
private:
  bool ConnectToServer();
private:
  std::string mServerIp;
  Socket mSocket;
  const std::unordered_map<std::string, ECommand> mCommandsMap
  {
    {"process_img", ECommand::PROCESS_IMAGE},
    {"srv_shutdown", ECommand::SERVER_SHUTDOWN},
    {"exit", ECommand::EXIT},

  };
  CommandHandler mCommandHandler;
};
