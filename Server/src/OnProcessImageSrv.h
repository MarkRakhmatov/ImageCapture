#pragma once
#include "Command.h"
#include "IOnCommandSrv.h"
#include <chrono>
#include <vector>

class OnProcessImageSrv: public IOnCommandSrv
{
public:
  ~OnProcessImageSrv() = default;
  EConnectionStatus Handle(Socket& sock);
private:
  std::chrono::milliseconds mTimeout{300};
};
