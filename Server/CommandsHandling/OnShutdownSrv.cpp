#include "OnShutdownSrv.h"

namespace ServerSide
{
	using namespace Communication;
	EConnectionStatus
	OnShutdownSrv::Handle(Socket& sock)
	{
	  EConnectionStatus response = EConnectionStatus::DISCONNECT;
	  bool res = sock.SendData(&response);
	  if(!res)
	  {
		  std::cout << "Failed to send status!" << std::endl;
	  }
	  return EConnectionStatus::SHUTDOWN;
	}
}
