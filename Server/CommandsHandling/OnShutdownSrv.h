#pragma once

#include "IOnCommandSrv.h"

class OnShutdownSrv : public IOnCommandSrv
{
public:
  EConnectionStatus Handle(Socket& sock);
};
