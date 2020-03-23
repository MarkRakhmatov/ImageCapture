#include "OnProcessImage.h"
#include "OnCapture.h"
#include "OnSetup.h"
#include "Command.h"

#include <iostream>
#include <unistd.h>

EConnectionStatus OnProcessImage::Handle(Socket& sock)
{
  auto comm = ECommand::PROCESS_IMAGE;
  auto sendRes = sock.SendData(&comm);
  if(!sendRes.second)
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
