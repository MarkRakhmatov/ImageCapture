#pragma once
#include "IOnCommand.h"
#include "OnCapture.h"
#include "OnSetup.h"

#include <unordered_map>
#include <memory>

class OnProcessImage : public IOnCommand
{
public:
  OnProcessImage();
  EConnectionStatus Handle(Socket& sock);
private:
  std::unique_ptr<IOnCommand>& GetCommandHandler();
private:
  std::unordered_map<std::string, std::unique_ptr<IOnCommand>> mProcessImageCommands{};
};
