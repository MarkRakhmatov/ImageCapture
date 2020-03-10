#pragma once
#include <string>
#include "DescriptorHolder.h"
#include <netinet/in.h>
#include "CommandHandler.h"
#include "Response.h"
#include "Socket.h"

class Client
{
public:
  Client(const std::string& serverIp = "192.168.7.2");
  EConnectionStatus Communicate();
private:
  DescriptorHolder OpenConnection();
  Socket mSocket;
  std::string mServerIp;
  CommandHandler mCommandHandler;
};
