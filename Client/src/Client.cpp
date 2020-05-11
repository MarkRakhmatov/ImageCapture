#include "Client.h"
#include "Command.h"
#include "Performance.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

namespace ClientSide
{
using namespace Communication;
	Client::Client(const std::string& serverIp)
	: mServerIp(serverIp)
	, mSocket(-1)
	{
	}

	EConnectionStatus
	HandleCommand(CommandHandler& handler, Socket& sock,ECommand command)
	{
		return handler.Handle(sock, command);
	}

	EConnectionStatus Client::Communicate()
	{
	  try
	  {
		for(;;)
		{
			ECommand command = GetCommand();
			EnsureConnection();
			std::chrono::milliseconds elapsedTime{};
			auto status =
					Performance(elapsedTime, HandleCommand, mCommandHandler, mSocket, command);
			std::cout << "Elapsed time (milliseconds): " << elapsedTime.count()<<std::endl;
			switch(status)
			{
			case EConnectionStatus::FAIL:
				std::cout << "Communication fail! Trying to reconnect to server!" << std::endl;
				mSocket.Reset();
				return status;
			case EConnectionStatus::DISCONNECT:
			case EConnectionStatus::SHUTDOWN:
				return status;
			case EConnectionStatus::SUCCESS:
				continue;
			}
		}
	  }
	  catch(const std::exception& ex)
	  {
		  std::cout << "Exception! Message:" << ex.what() << std::endl;
	  }
	  return EConnectionStatus::FAIL;
	}

	ECommand
	Client::GetCommand()
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

	void Client::EnsureConnection()
	{
		if(!IsConnected())
		{
			if(!ConnectToServer())
			{
				std::cout<< "Failed to connect to server!" <<std::endl;
			}
		}
	}

	bool Client::IsConnected()
	{
		return mSocket.Get()>0;
	}

	bool Client::ConnectToServer()
	{
		int sockDesk = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in mServAddr{};
		mServAddr.sin_family = AF_INET;
		mServAddr.sin_port = htons(3425);

		if(inet_pton(AF_INET, mServerIp.data(), &mServAddr.sin_addr)<=0)
		{
			std::cout<<"Failed to convert server ip to binary format!"<<std::endl;
			return false;
		}

		if(connect(sockDesk, reinterpret_cast<sockaddr*>(&mServAddr), sizeof(mServAddr)) < 0)
		{
			std::cout<<"Failed to open connection!"<<std::endl;
			return false;
		}

		mSocket.Reset(std::move(sockDesk));
		return true;
	}
}
