#include "Client.h"
#include "Command.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

Client::Client(const std::string& serverIp)
: mSocket(serverIp)
{
}

EConnectionStatus Client::Communicate()
{
  try
  {
    for(;;)
    {
        auto status = mCommandHandler.Handle(mSocket);
        if(status == EConnectionStatus::FAIL)
        {
            std::cout << "Communication fail! Trying to reconnect to server!" << std::endl;
            break;
        }
        if(status == EConnectionStatus::DISCONNECT )
        {
            return status;
        }
        if(status == EConnectionStatus::SHUTDOWN )
        {
            return status;
        }
    }
  }
  catch(const std::exception& ex)
  {
      std::cout << "Exception! Message:" << ex.what() << std::endl;
  }
  return EConnectionStatus::FAIL;
}

