#pragma once

#include "IOnCommand.h"

class OnServerShutdown : public IOnCommand
{
public:
  EConnectionStatus Handle(Socket& sock);
private:
  EConnectionStatus GetResponse(Socket& sock);
};
