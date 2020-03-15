#pragma once

#include <IOnCommand.h>

class OnSetup : public IOnCommand
{
  EConnectionStatus Handle(Socket& sock);
private:
  EConnectionStatus SendRequest(Socket& sock);
  EConnectionStatus GetResponse(Socket& sock);
};
