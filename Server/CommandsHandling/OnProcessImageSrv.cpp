#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include "VideoDevice/VideoDevice.h"
#include <sstream>
#include "ImageProcessing/JpegHelper.h"
#include "ImageProcessing/ImageProcessor.h"

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
    std::string fileName("Images/webcam_output_");
    auto decomprBuffer = GetImageBufferFromDevice(mDevice);
    EConnectionStatus response = EConnectionStatus::FAIL;
    if(!decomprBuffer.GetWidth() || !decomprBuffer.GetHeight())
    {
        std::cout<<"Failed to get buffer!"<<std::endl;
        sock.SendData(&response);
        break;
    }
    ImageProcessor::ToGrayScale(decomprBuffer);
    auto compressedBuffer = JpegHelper::Compress(decomprBuffer);
    response = EConnectionStatus::SUCCESS;
    auto res = sock.SendData(&response);
    if(!res.second)
    {
        break;
    }
    int32_t x = 73;
    int32_t y = 27;
    res = sock.SendData(&x, &y);
    if(!res.second)
    {
        break;
    }

    std::ostringstream ss;
    ss << fileName << mImagesCount++ << ".jpeg";
    JpegHelper::WriteBufferToFile(compressedBuffer, ss.str());

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
