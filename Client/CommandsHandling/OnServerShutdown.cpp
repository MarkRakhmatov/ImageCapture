#include "OnServerShutdown.h"

EConnectionStatus
OnServerShutdown::Handle(Socket& sock)
{
  return GetResponse(sock);
}

EConnectionStatus
OnServerShutdown::GetResponse(Socket& sock)
{
  EConnectionStatus status = EConnectionStatus::FAIL;
  sock.ReadData(&status);
  if(status != EConnectionStatus::DISCONNECT)
  {
      std::cout << "Failed to shutdown server!" <<std::endl;
      return status;
  }
  std::cout << "Server shut down successfully!" <<std::endl;
  return status;
}
