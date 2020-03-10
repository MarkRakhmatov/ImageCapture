#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Socket.h"

class IOnCommand
{
public:
  virtual ~IOnCommand(){};
  virtual void Handle(Socket& sock) = 0;
};
