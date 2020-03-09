#pragma once
#include <string>
#include "DescriptorHolder.h"
#include <netinet/in.h>
#include "CommandHandler.h"
#include "Response.h"

class Client
{
public:
  Client(const std::string& serverIp = "192.168.7.2");
  EResponse Communicate();
private:
  DescriptorHolder OpenConnection();
  std::string mServerIp;
  sockaddr_in mServAddr{};
  CommandHandler mCommandHandler;
};
