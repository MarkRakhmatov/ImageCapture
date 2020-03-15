#include "OnCapture.h"
#include "Command.h"
#include "Helper.h"

EConnectionStatus
OnCapture::Handle(Socket& sock)
{
  EConnectionStatus status = SendRequest(sock);
  if(status == EConnectionStatus::FAIL)
  {
      return status;
  }
  return GetResponse(sock);
}

EConnectionStatus
OnCapture::SendRequest(Socket& sock)
{
  EProcessImage command = EProcessImage::PROCESS_IMAGE;

  auto res = sock.Send(&command, sizeof(command));
  if(!res.second)
  {
      return EConnectionStatus::FAIL;
  }
  return EConnectionStatus::SUCCESS;
}

EConnectionStatus
OnCapture::GetResponse(Socket& sock)
{
  EConnectionStatus status = ReadStatus(sock);

  if(status == EConnectionStatus::FAIL)
  {
      return status;
  }

  int32_t x = 0;
  int32_t y = 0;
  auto res = sock.ReadData(&x, &y);
  if(!res.second)
  {
      return EConnectionStatus::FAIL;
  }
  std::cout << "x = " << x << std::endl
        << "y = " << y << std::endl;
  return status;
}
