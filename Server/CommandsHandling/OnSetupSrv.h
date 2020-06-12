#pragma once
#include "IOnCommandSrv.h"
#include "IImageSource.h"
#include <memory>

namespace ServerSide
{
	class OnSetupSrv : public IOnCommandSrv
	{
	public:
		OnSetupSrv(std::unique_ptr<IImageSource< unsigned char>>& imageSource);
		Communication::EConnectionStatus Handle(Communication::Socket& sock);
	private:
		std::unique_ptr<IImageSource<unsigned char>>& mImageSource;
	};
}
