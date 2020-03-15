#include "OnSetup.h"
#include "Command.h"

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

  auto res = sock.SendData(&command);
  if(!res.second)
  {
      std::cout << "Failed to send command!" << std::endl;
      return EConnectionStatus::FAIL;
  }
  return EConnectionStatus::SUCCESS;
}

EConnectionStatus
OnSetup::GetResponse(Socket& sock)
{
  EConnectionStatus status = EConnectionStatus::FAIL;
  sock.ReadData(&status);
  if(status == EConnectionStatus::FAIL)
  {
      std::cout << "Failed to setup camera!" << std::endl;
      return status;
  }

  std::cout << "Successfully setup camera!" << std::endl;
  return status;
}

