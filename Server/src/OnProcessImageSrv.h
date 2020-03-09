#pragma once
#include "Command.h"
#include "IOnCommandSrv.h"
#include <vector>

class OnProcessImageSrv: public IOnCommandSrv
{
public:
  ~OnProcessImageSrv() = default;
  void Handle(DescriptorHolder& sock);
};
