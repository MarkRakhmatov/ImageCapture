#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include "ServerCommandHandler.h"
#include "OnProcessImageSrv.h"

CommandHandlerData ServerCommandHandler::mData{};

ServerCommandHandler::ServerCommandHandler()
{
  // TODO add ECommand::SETUP_CAMERA command handling
  //mCommandToHandler[static_cast<size_t>(ECommand::SETUP_CAMERA)] = &OnSetupCommand;
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
      return EConnectionStatus::FAIL;
  }
  if(command >= ECommand::SIZE)
  {
      std::cout << "Unknown command!" << std::endl;
      return EConnectionStatus::UNKNOWN_COMMAND;
  }
  if(command == ECommand::SHUTDOWN)
  {
      std::cout << "Shutdown server!" << std::endl;
      return EConnectionStatus::SHUTDOWN;
  }
  auto& handler = mCommandToHandler[static_cast<size_t>(command)];
  if(!handler)
  {
      std::cout << "Command handler is not presented!" << std::endl;
      return EConnectionStatus::UNKNOWN_COMMAND;
  }
  return handler->Handle(sock);
}

CommandHandlerData&
ServerCommandHandler::GetData()
{
  return mData;
}
