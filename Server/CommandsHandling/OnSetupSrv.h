#pragma once
#include "IOnCommandSrv.h"

class OnSetupSrv : public IOnCommandSrv
{
public:
  EConnectionStatus Handle(Socket& sock);
};
