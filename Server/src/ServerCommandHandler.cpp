#include <netinet/in.h>
#include <sys/socket.h>
#include "ServerCommandHandler.h"
#include "OnProcessImageSrv.h"

CommandHandlerData ServerCommandHandler::mData{};

ServerCommandHandler::ServerCommandHandler()
{
  // TODO add ECommand::SETUP_CAMERA command handling
  //mCommandToHandler[static_cast<size_t>(ECommand::SETUP_CAMERA)] = &OnSetupCommand;
  mCommandToHandler[static_cast<size_t>(ECommand::PROCESS_IMAGE)].reset({new OnProcessImageSrv});
}

EResponse
ServerCommandHandler::Handle(DescriptorHolder& sock)
{
  ECommand command;
  int bytesRead = recv(sock.Get(), &command, sizeof(command), 0);
  if(bytesRead <= 0 || command >= ECommand::SIZE)
  {
      return EResponse::FAIL;
  }
  mCommandToHandler[static_cast<size_t>(command)]->Handle(sock);
  return EResponse::SUCCESS;
}

CommandHandlerData&
ServerCommandHandler::GetData()
{
  return mData;
}
