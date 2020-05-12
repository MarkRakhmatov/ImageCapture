#pragma once
#include "IOnCommandSrv.h"
#include "VideoDevice/VideoDevice.h"

namespace ServerSide
{
	class OnCaptureSrv: public IOnCommandSrv
	{
	public:
		explicit OnCaptureSrv(VideoDevice& device);
		Communication::EConnectionStatus Handle(Communication::Socket& sock);
	private:
		VideoDevice& mDevice;
	};

}
