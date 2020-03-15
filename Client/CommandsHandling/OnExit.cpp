#include "OnExit.h"
#include "Helper.h"

EConnectionStatus
OnExit::Handle(Socket& sock)
{
  return GetResponse(sock);
}

EConnectionStatus
OnExit::GetResponse(Socket& sock)
{
  ReadStatus(sock);

  std::cout << "Closing client!" << std::endl;
  return EConnectionStatus::DISCONNECT;
}
