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
        std::string message = "Failed to bind socked and address";
        perror(message.c_str());
        throw std::runtime_error{message};
    }
}

void
Server::Listen()
{
  listen(mListener.Get(), 1);

  while(1)
  {
      DescriptorHolder sock = accept(mListener.Get(), NULL, NULL);
      if(sock.Get() < 0)
      {
          std::string message = "Failed to accept connection";
          perror(message.c_str());
          continue;
      }
      auto status = mCommandHandler.Handle(sock);
      std::cout << "Handle command with status: "
          << static_cast<size_t>(status);
  }
}

int
Server::HandleCommand(DescriptorHolder& sock)
{
  mCommandHandler.Handle(sock);
  return 1;
}
