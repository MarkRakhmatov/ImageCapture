#pragma once

#include "IOnCommand.h"

class OnExit : public IOnCommand
{
public:
  EConnectionStatus Handle(Socket& sock);
};
