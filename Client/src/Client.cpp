#include "Client.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "Command.h"

Client::Client(const std::string& serverIp)
: mSocket(serverIp)
{
}

EConnectionStatus Client::Communicate()
{
  try
  {
    int comm;
    std::cout << "Enter command: ";
    std::cin >> comm;
    ECommand command = static_cast<ECommand>(comm);
    if(command >= ECommand::SIZE)
    {
        std::cout << "Invalid command!" <<std::endl;
        return EConnectionStatus::FAIL;
    }
    mCommandHandler.Handle(mSocket, command);
    return EConnectionStatus::SUCCESS;
  }
  catch(const std::exception& ex)
  {
      std::cout << "Exception! Message:" << ex.what() << std::endl;
  }
  return EConnectionStatus::FAIL;
}
