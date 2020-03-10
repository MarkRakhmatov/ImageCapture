#pragma once
#include <Command.h>
#include <netinet/in.h>
#include "DescriptorHolder.h"
#include "ServerCommandHandler.h"
#include "Socket.h"

class Server
{
public:
  Server();
  EConnectionStatus Listen();
private:
  EConnectionStatus HandleCommand();
  DescriptorHolder mListener;
  sockaddr_in mAddr{};
  ServerCommandHandler mCommandHandler;
};
