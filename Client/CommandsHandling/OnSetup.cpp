#include "OnSetup.h"
#include "Command.h"
#include "Helper.h"

EConnectionStatus
OnSetup::Handle(Socket& sock)
{
  EConnectionStatus status = SendRequest(sock);
  if(status == EConnectionStatus::FAIL)
  {
      return status;
  }
  return GetResponse(sock);
}

EConnectionStatus
OnSetup::SendRequest(Socket& sock)
{
  EProcessImage command = EProcessImage::SETUP_CAMERA;

  auto res = sock.Send(&command, sizeof(command));
  if(!res.second)
  {
      return EConnectionStatus::FAIL;
  }
  return EConnectionStatus::SUCCESS;
}

EConnectionStatus
OnSetup::GetResponse(Socket& sock)
{
  EConnectionStatus status = ReadStatus(sock);

  return status;
}

