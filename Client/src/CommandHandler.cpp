#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
//#include "OnSetup.h"
#include "OnProcessImage.h"
#include "CommandHandler.h"
#include "CommandHandler.h"

CommandHandler::CommandHandler()
{
  //TODO add setup command handling
  //mCommandToHandler[static_cast<size_t>(ECommand::SETUP_CAMERA)] = new OnProcessImage;
  mCommandToHandler[static_cast<size_t>(ECommand::PROCESS_IMAGE)].reset(new OnProcessImage);
}

EResponse
CommandHandler::Handle(DescriptorHolder& sock, ECommand command)
{
  send(sock.Get(), &command, sizeof(command), 0);
  EResponse status;
  int bytesRead = read(sock.Get(), &status, sizeof(status));

  std::cout <<
      "Operation status: " <<
      static_cast<size_t>(status) << std::endl;

  if(bytesRead < 0 || status != EResponse::SUCCESS)
  {
      printf("Failed to receive response from server");
      return EResponse::FAIL;
  }
  mCommandToHandler[static_cast<size_t>(command)]->Handle(sock);

  return EResponse::SUCCESS;
}
