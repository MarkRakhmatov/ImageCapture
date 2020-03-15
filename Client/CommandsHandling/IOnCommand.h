#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Socket.h"
#include "Response.h"

class IOnCommand
{
public:
  virtual ~IOnCommand(){};
  virtual EConnectionStatus Handle(Socket& sock) = 0;
};

