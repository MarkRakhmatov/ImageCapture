#pragma once
#include "Command.h"
#include "IOnCommandSrv.h"
#include "IImageSource.h"

#include <chrono>
#include <vector>
#include <array>
#include <memory>

namespace ServerSide
{
	class OnProcessImageSrv: public IOnCommandSrv
	{
	public:
	  OnProcessImageSrv();
	  ~OnProcessImageSrv() = default;
	  Communication::EConnectionStatus Handle(Communication::Socket& sock);
	private:
	  Communication::EProcessImage ReadCommand(Communication::Socket& sock);
	  std::unique_ptr<IOnCommandSrv>& GetCommandHandler(Communication::EProcessImage command);
	private:
	  std::unique_ptr<IImageSource<unsigned char>> mImageSource;
	  std::chrono::milliseconds mTimeout{300};
	  std::array<std::unique_ptr<IOnCommandSrv>,  static_cast<size_t>(Communication::EProcessImage::SIZE)> mProcessImageCommands{};
	};
}
