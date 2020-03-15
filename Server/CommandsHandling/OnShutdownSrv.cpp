#include "OnShutdownSrv.h"

EConnectionStatus
OnShutdownSrv::Handle(Socket& sock)
{
  EConnectionStatus response = EConnectionStatus::DISCONNECT;
  auto res = sock.SendData(&response);
  if(!res.second)
  {
      std::cout << "Failed to send status!" << std::endl;
  }
  return EConnectionStatus::SHUTDOWN;
}
