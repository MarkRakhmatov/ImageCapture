#include "VideoDevice/VideoDevice.h"
#include "ImageProcessing/JpegHelper.h"
#include "ImageProcessing/ImageProcessor.h"
#include "SettingsHandler.h"
#include "OnCaptureSrv.h"
#include "OnSetupSrv.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include "DeviceImageSource.h"

#include "OnProcessImageSrv.h"

namespace ServerSide
{
	using namespace Communication;
	EConnectionStatus
	OnProcessImageSrv::Handle(Socket& sock)
	{
	  EProcessImage comm = ReadCommand(sock);
	  if(comm == EProcessImage::SIZE)
	  {
		  EConnectionStatus response = EConnectionStatus::FAIL;
		  std::cout<<"Unsupported image processing command!"<<std::endl;
		  sock.SendData(&response);
		  return response;
	  }
	  return mProcessImageCommands[static_cast<size_t>(comm)]->Handle(sock);
	}

	OnProcessImageSrv::OnProcessImageSrv()
		: mImageSource(new DeviceImageSource("/dev/video0", 1920, 1080))
	{
		mProcessImageCommands[static_cast<size_t>(EProcessImage::CAPTURE)] = std::unique_ptr<IOnCommandSrv>(new OnCaptureSrv(mImageSource));
		mProcessImageCommands[static_cast<size_t>(EProcessImage::SETUP_CAMERA)] = std::unique_ptr<IOnCommandSrv>(new OnSetupSrv(mImageSource));
	}

	EProcessImage
	OnProcessImageSrv::ReadCommand(Socket& sock)
	{
	  EProcessImage command = EProcessImage::SIZE;
	  bool res = sock.ReadData(&command);
	  if(!res || command > EProcessImage::SIZE)
	  {
		  return EProcessImage::SIZE;
	  }
	  return command;
	}
}
