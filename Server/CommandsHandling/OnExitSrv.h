#pragma once
#include "IOnCommandSrv.h"
class OnExitSrv: public IOnCommandSrv
{
public:
  EConnectionStatus Handle(Socket& sock);
};
