#pragma once
#include "IOnCommand.h"

class OnCapture : public IOnCommand
{
  EConnectionStatus Handle(Socket& sock);
private:
  EConnectionStatus SendRequest(Socket& sock);
  EConnectionStatus GetResponse(Socket& sock);
};
