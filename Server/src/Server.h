#pragma once
#include <Command.h>
#include <netinet/in.h>
#include "DescriptorHolder.h"

#include "ServerCommandHandler.h"

class Server
{
public:
  Server();
  void Listen();
private:
  int HandleCommand(DescriptorHolder& sock);
  DescriptorHolder mListener;
  sockaddr_in mAddr;
  ServerCommandHandler mCommandHandler;
};
