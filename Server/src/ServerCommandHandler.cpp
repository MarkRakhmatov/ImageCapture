#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include "ServerCommandHandler.h"

#include "OnShutdownSrv.h"
#include "OnProcessImageSrv.h"

namespace ServerSide
{
	using namespace Communication;
	ServerCommandHandler::ServerCommandHandler()
	{
	  mCommandToHandler[static_cast<size_t>(ECommand::SERVER_SHUTDOWN)].reset({new OnShutdownSrv});
	  mCommandToHandler[static_cast<size_t>(ECommand::PROCESS_IMAGE)].reset({new OnProcessImageSrv});
	}

	EConnectionStatus
	ServerCommandHandler::Handle(Socket& sock)
	{
	  ECommand command;
	  bool res = sock.ReadData(&command);

	  if(!res)
	  {
		  std::cout << "Failed to receive command!" << std::endl;
		  return EConnectionStatus::DISCONNECT;
	  }
	  if(command >= ECommand::SIZE)
	  {
		  std::cout << "Unknown command!" << std::endl;
		  return EConnectionStatus::DISCONNECT;
	  }
	  auto& handler = mCommandToHandler[static_cast<size_t>(command)];
	  if(!handler)
	  {
		  std::cout << "Command handler is not presented!" << std::endl;
		  return EConnectionStatus::DISCONNECT;
	  }
	  return handler->Handle(sock);
	}
}
