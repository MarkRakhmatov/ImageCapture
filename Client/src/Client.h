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
private:
  Socket mSocket;
  std::string mServerIp;
  CommandHandler mCommandHandler;
};
