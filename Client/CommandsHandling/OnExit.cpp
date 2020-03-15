#include "OnExit.h"

EConnectionStatus
OnExit::Handle(Socket& sock)
{
  return GetResponse(sock);
}

EConnectionStatus
OnExit::GetResponse(Socket& sock)
{
  EConnectionStatus status = EConnectionStatus::FAIL;
  sock.ReadData(&status);
  if(status == EConnectionStatus::FAIL)
  {
      std::cout << "Failed to read status. Something wrong with connection!" << std::endl;
  }
  std::cout << "Closing client!" << std::endl;
  return EConnectionStatus::DISCONNECT;
}
