#include "VideoDevice/VideoDevice.h"
#include "ImageProcessing/JpegHelper.h"
#include "ImageProcessing/ImageProcessor.h"
#include "SettingsHandler.h"
#include "SwapChain.h"
#include "Calculations.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

#include "OnProcessImageSrv.h"

EConnectionStatus
OnProcessImageSrv::Handle(Socket& sock)
{
  do
  {
    EProcessImage comm = ReadCommand(sock);
    switch(comm)
    {
    case EProcessImage::SETUP_CAMERA:
    {
      return SetupCamera(sock);
    }
    case EProcessImage::PROCESS_IMAGE:
    {
      return ProcessImage(sock);
    }
    default:
    {
      EConnectionStatus response = EConnectionStatus::FAIL;
      std::cout<<"Unsupported image processing command!"<<std::endl;
      sock.SendData(&response);
      break;
    }
    };
  }
  while(false);

  std::cout<<"Failed to send response!"<<std::endl;
  return EConnectionStatus::FAIL;
}

EConnectionStatus
OnProcessImageSrv::SetupCamera(Socket& sock)
{
  mDevice.HandleParameters();
  auto response = EConnectionStatus::SUCCESS;
  auto res = sock.SendData(&response);
  if(!res.second)
  {
      return EConnectionStatus::FAIL;
  }
  return EConnectionStatus::SUCCESS;
}

EConnectionStatus
OnProcessImageSrv::ProcessImage(Socket& sock)
{
  do
  {
    std::string fileName("Images/img_");
    auto decomprBuffer = GetImageBufferFromDevice(mDevice);
    EConnectionStatus response = EConnectionStatus::FAIL;
    if(!decomprBuffer.GetWidth() || !decomprBuffer.GetHeight())
    {
        std::cout<<"Failed to get buffer!"<<std::endl;
        sock.SendData(&response);
        break;
    }
    auto& settings = SettingsHandler::Get().GetSettings();
    settings.clear();
    GetSettings(sock);

    auto width = decomprBuffer.GetWidth();
    auto height = decomprBuffer.GetHeight();
    ImageBuffer<unsigned char> resultBuffer(width, height, decomprBuffer.GetPixelType());
    SwapChain<ImageBuffer<unsigned char>> chain(&decomprBuffer, &resultBuffer);
    for(size_t i = 0; i < settings.size(); ++i)
    {
        ConvHandler<unsigned char> convHandler
        (
        		settings[i]
        );
        auto& originalBuff = *chain.GetActive();
        chain.Swap();
        auto& processedBuff = *chain.GetActive();
        ImageProcessor::Convolution(
        		originalBuff,
				processedBuff,
                convHandler);
    }

    auto compressedBuffer = JpegHelper::Compress(*chain.GetActive());
    response = EConnectionStatus::SUCCESS;
    auto res = sock.SendData(&response);
    if(!res.second)
    {
        break;
    }
    int32_t x{};
    int32_t y{};
    auto status = GetAngles(*chain.GetActive(), x, y);
    res = sock.SendData(&x, &y);
    if(!res.second)
    {
        break;
    }

    JpegHelper::WriteBufferToFile(compressedBuffer, fileName);

    return EConnectionStatus::SUCCESS;
  }
  while(false);

  std::cout<<"Failed to send response!"<<std::endl;
  return EConnectionStatus::FAIL;
}

EProcessImage
OnProcessImageSrv::ReadCommand(Socket& sock)
{
  EProcessImage command = EProcessImage::EMPTY_COMMAND;
  auto res = sock.ReadData(&command);
  if(!res.second)
  {
      return EProcessImage::EMPTY_COMMAND;
  }
  return command;
}
