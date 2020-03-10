#pragma once
#include "DescriptorHolder.h"
#include "Socket.h"
#include "Response.h"
class IOnCommandSrv
{
public:
  virtual ~IOnCommandSrv(){};
  virtual EConnectionStatus Handle(Socket& sock) = 0;
};
