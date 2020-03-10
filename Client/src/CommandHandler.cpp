#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
//#include "OnSetup.h"
#include "OnProcessImage.h"
#include "CommandHandler.h"
#include "Command.h"

CommandHandler::CommandHandler()
{
  //TODO add setup command handling
  //mCommandToHandler[static_cast<size_t>(ECommand::SETUP_CAMERA)] = new OnProcessImage;
  mCommandToHandler[static_cast<size_t>(ECommand::PROCESS_IMAGE)].reset(new OnProcessImage);
}

void
CommandHandler::Handle(Socket& sock, ECommand command)
{
  auto sendRes = sock.Send(&command, sizeof(command));
  if(!sendRes.second)
  {
      throw std::runtime_error("Failed to send command to server");
  }
  EConnectionStatus status;
  auto res = sock.Read(&status, sizeof(status));

  std::cout <<
      "Operation status: " <<
      static_cast<size_t>(status) << std::endl;

  if(!res.second || status != EConnectionStatus::SUCCESS)
  {
      throw std::runtime_error("Failed to receive response from server");
  }

  mCommandToHandler[static_cast<size_t>(command)]->Handle(sock);
}
