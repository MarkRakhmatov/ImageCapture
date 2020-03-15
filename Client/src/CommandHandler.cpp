#include "CommandHandler.h"
#include "Command.h"
#include "OnProcessImage.h"
#include "OnExit.h"
#include "OnServerShutdown.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>

CommandHandler::CommandHandler()
{
  mCommandToHandler[static_cast<size_t>(ECommand::PROCESS_IMAGE)]
                    .reset(new OnProcessImage);
  mCommandToHandler[static_cast<size_t>(ECommand::EXIT)]
                    .reset(new OnExit);
  mCommandToHandler[static_cast<size_t>(ECommand::SERVER_SHUTDOWN)]
                    .reset(new OnServerShutdown);
}

ECommand
CommandHandler::GetCommand()
{
  std::cout << "Enter command: ";
  std::string command;
  std::cin >> command;
  auto iter = mCommandsMap.find(command);
  if(iter == mCommandsMap.end())
  {
      return ECommand::SIZE;
  }
  return iter->second;
}

EConnectionStatus
CommandHandler::Handle(Socket& sock)
{
  ECommand command = GetCommand();
  if(command >= ECommand::SIZE)
  {
      std::cout << "Invalid command!" <<std::endl;
      return EConnectionStatus::FAIL;
  }

  auto sendRes = sock.Send(&command, sizeof(command));
  if(!sendRes.second)
  {
      throw std::runtime_error("Failed to send command to server");
  }

  return mCommandToHandler[static_cast<size_t>(command)]->Handle(sock);
}
