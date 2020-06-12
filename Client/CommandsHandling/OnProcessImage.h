#pragma once
#include "IOnCommand.h"

#include <unordered_map>
#include <memory>

namespace ClientSide
{
	class OnProcessImage : public IOnCommand
	{
	public:
	  OnProcessImage();
	  Communication::EConnectionStatus Handle(Communication::Socket& sock);
	private:
	  std::unique_ptr<IOnCommand>& GetCommandHandler();
	private:
	  std::unordered_map<std::string, std::unique_ptr<IOnCommand>> mProcessImageCommands{};
	};
}
