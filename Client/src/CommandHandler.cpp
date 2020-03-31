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

EConnectionStatus
CommandHandler::Handle(Socket& sock, ECommand command)
{
  if(command >= ECommand::SIZE)
  {
      std::cout << "Invalid command!" << std::endl;
      return EConnectionStatus::FAIL;
  }

  return mCommandToHandler[static_cast<size_t>(command)]->Handle(sock);
}
