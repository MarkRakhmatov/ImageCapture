#include "OnProcessImage.h"
#include "OnCapture.h"
#include "OnSetup.h"
#include "Command.h"

#include <iostream>
#include <unistd.h>

namespace ClientSide
{
using namespace Communication;

	EConnectionStatus OnProcessImage::Handle(Socket& sock)
	{
	  auto comm = ECommand::PROCESS_IMAGE;
	  bool res = sock.SendData(&comm);
	  if(!res)
	  {
		  return EConnectionStatus::FAIL;
	  }
	  return GetCommandHandler()->Handle(sock);
	}

	OnProcessImage::OnProcessImage()
	{
	  mProcessImageCommands["setup"].reset(new OnSetup);
	  mProcessImageCommands["capture"].reset(new OnCapture);
	}

	std::unique_ptr<IOnCommand>& OnProcessImage::GetCommandHandler()
	{
	  std::string comm;
	  std::cin >> comm;
	  auto iter = mProcessImageCommands.find(comm);
	  if(iter == mProcessImageCommands.end())
	  {
		  throw std::runtime_error("Invalid image processing command");
	  }
	  return iter->second;
	}
}
