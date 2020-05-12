#include "OnCapture.h"
#include "Command.h"

namespace ClientSide
{
	using namespace Communication;

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
	  EProcessImage command = EProcessImage::CAPTURE;

	  bool res = sock.SendData(&command);
	  if(!res)
	  {
		  return EConnectionStatus::FAIL;
	  }
	  return EConnectionStatus::SUCCESS;
	}

	EConnectionStatus
	OnCapture::GetResponse(Socket& sock)
	{
	  EConnectionStatus status = EConnectionStatus::FAIL;
	  sock.ReadData(&status);

	  if(status == EConnectionStatus::FAIL)
	  {
		  return status;
	  }

	  int32_t x = 0;
	  int32_t y = 0;

	  bool res = sock.ReadData(&x, &y);
	  if(!res)
	  {
		  return EConnectionStatus::FAIL;
	  }
	  std::cout << "x = " << x << std::endl;
	  std::cout << "y = " << y << std::endl;
	  return status;
	}
}
