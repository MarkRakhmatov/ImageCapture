#include "OnCapture.h"
#include "Command.h"
#include "CommunicationUtils.h"
#include <sstream>

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

	  float x = 0;
	  float y = 0;
	  std::stringstream converter;
	  std::string fi, gamma;
	  bool res = Communication::ReadString(sock, fi);
	  if(!res)
	  {
		  return EConnectionStatus::FAIL;
	  }
	  res = Communication::ReadString(sock, gamma);
	  if(!res)
	  {
		  return EConnectionStatus::FAIL;
	  }
	  converter << fi << " "<<gamma;
	  converter >> x >> y;
	  std::cout << "x = " << x << std::endl;
	  std::cout << "y = " << y << std::endl;
	  return status;
	}
}
