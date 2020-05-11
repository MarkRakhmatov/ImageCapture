#pragma once
#include "Command.h"
#include "IOnCommandSrv.h"
#include "VideoDevice/VideoDevice.h"
#include <chrono>
#include <vector>

namespace ServerSide
{
	class OnProcessImageSrv: public IOnCommandSrv
	{
	public:
	  ~OnProcessImageSrv() = default;
	  Communication::EConnectionStatus Handle(Communication::Socket& sock);
	private:
	  Communication::EConnectionStatus SetupCamera(Communication::Socket& sock);
	  Communication::EConnectionStatus ProcessImage(Communication::Socket& sock);
	  Communication::EProcessImage ReadCommand(Communication::Socket& sock);
	private:
	  std::string mDeviceName{"/dev/video0"};
	  VideoDevice mDevice{mDeviceName};
	  size_t mImagesCount = 0;
	  std::chrono::milliseconds mTimeout{300};
	};
}
