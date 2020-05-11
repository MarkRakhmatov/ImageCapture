#include "OnServerShutdown.h"
#include "Command.h"

namespace ClientSide
{
using namespace Communication;
	EConnectionStatus
	OnServerShutdown::Handle(Socket& sock)
	{
	  auto comm = ECommand::SERVER_SHUTDOWN;
	  bool res = sock.SendData(&comm);
	  if(!res)
	  {
		  return EConnectionStatus::FAIL;
	  }
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
}
