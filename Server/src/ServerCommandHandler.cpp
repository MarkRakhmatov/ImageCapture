#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include "ServerCommandHandler.h"

#include "OnShutdownSrv.h"
#include "OnExitSrv.h"
#include "OnProcessImageSrv.h"

ServerCommandHandler::ServerCommandHandler()
{
  mCommandToHandler[static_cast<size_t>(ECommand::EXIT)].reset({new OnExitSrv});
  mCommandToHandler[static_cast<size_t>(ECommand::SERVER_SHUTDOWN)].reset({new OnShutdownSrv});
  mCommandToHandler[static_cast<size_t>(ECommand::PROCESS_IMAGE)].reset({new OnProcessImageSrv});
}

EConnectionStatus
ServerCommandHandler::Handle(Socket& sock)
{
  ECommand command;
  auto res = sock.Read(&command, sizeof(command));

  if(!res.second)
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
