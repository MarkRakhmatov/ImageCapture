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
: mServerIp(serverIp)
{
  mServAddr.sin_family = AF_INET;
  mServAddr.sin_port = htons(3425);
}

EResponse Client::Communicate()
{
  int comm;
  std::cout << "Enter command: ";
  std::cin >> comm;
  ECommand command = static_cast<ECommand>(comm);
  if(command >= ECommand::SIZE)
  {
      std::cout << "Invalid command!" <<std::endl;
      return EResponse::FAIL;
  }
  DescriptorHolder sock = OpenConnection();
  return mCommandHandler.Handle(sock, command);

}

DescriptorHolder
Client::OpenConnection()
{
  DescriptorHolder sock{socket(AF_INET, SOCK_STREAM, 0)};


  if(inet_pton(AF_INET, mServerIp.data(), &mServAddr.sin_addr)<=0)
  {
      throw std::runtime_error("inet_pton error occurred");
  }

  if(connect(sock.Get(), reinterpret_cast<sockaddr*>(&mServAddr), sizeof(mServAddr)) < 0)
  {
      printf("Error : Failed to connect!");
      return 0;
  }
  return sock;
}
