#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include "Server.h"


Server::Server()
  : mListener(socket(AF_INET, SOCK_STREAM, 0))
{
    mAddr.sin_family = AF_INET;
    mAddr.sin_port = htons(3425);
    mAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(mListener.Get(), reinterpret_cast<sockaddr*>(&mAddr), sizeof(mAddr)) < 0)
    {
        std::cout << "Failed to bind socked and address" << std::endl;
        mListener.Reset();
    }
}

EConnectionStatus
Server::Listen()
{
    if(listen(mListener.Get(), 1)<0)
    {
        std::cout << "Failed to start listening!" << std::endl;
        return EConnectionStatus::FAIL;
    }

    for(;;)
    {
        EConnectionStatus status = HandleCommand();
        switch(status)
        {
        case EConnectionStatus::FAIL:
          continue;
        case EConnectionStatus::SHUTDOWN:
          return status;
        case EConnectionStatus::DISCONNECT:
          return status;
        default:
          continue;
        }
    }
  return EConnectionStatus::SHUTDOWN;
}

EConnectionStatus
Server::HandleCommand()
{
  Socket sock{accept(mListener.Get(), NULL, NULL)};
  for(;;)
  {
      EConnectionStatus status = mCommandHandler.Handle(sock);
      switch(status)
      {
      case EConnectionStatus::DISCONNECT:
        break;
      case EConnectionStatus::SHUTDOWN:
        return status;
      default:
        continue;
      }
      break;
  }
  return EConnectionStatus::FAIL;
}
