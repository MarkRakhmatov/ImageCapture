#pragma once

#include "IOnCommand.h"

class OnExit : public IOnCommand
{
public:
  EConnectionStatus Handle(Socket& sock);
private:
  EConnectionStatus GetResponse(Socket& sock);
};
