#pragma once
#include "DescriptorHolder.h"

class IOnCommandSrv
{
public:
  virtual ~IOnCommandSrv(){};
  virtual void Handle(DescriptorHolder& sock) = 0;
};
