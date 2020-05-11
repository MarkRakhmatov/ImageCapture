#pragma once
#include "CommandHandler.h"
#include "Command.h"
#include "Response.h"
#include "Socket.h"

#include <string>
#include <netinet/in.h>

namespace ClientSide
{
	class Client
	{
	public:
	  Client(const std::string& serverIp = "192.168.7.2");
	  Communication::EConnectionStatus Communicate();
	  Communication::ECommand GetCommand();
	  void EnsureConnection();
	  bool IsConnected();
	private:
	  bool ConnectToServer();
	private:
	  std::string mServerIp;
	  Communication::Socket mSocket;
	  const std::unordered_map<std::string, Communication::ECommand> mCommandsMap
	  {
		{"process_img", Communication::ECommand::PROCESS_IMAGE},
		{"srv_shutdown", Communication::ECommand::SERVER_SHUTDOWN},
		{"exit", Communication::ECommand::EXIT},

	  };
	  CommandHandler mCommandHandler;
	};
}
