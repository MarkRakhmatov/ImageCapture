#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include "VideoDevice/VideoDevice.h"
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
bool GetSettings(Socket& sock, ConvKernel& ker)
{
  uint32_t size{};
  auto res = sock.ReadData(&size);
  do
  {
     if(!res.second)
     {
         break;
     }
     std::string kernel(size, '\0');
     for(uint32_t i = 0; i < size; ++i)
     {
         char tmp{};
         res = sock.ReadData(&tmp);
         kernel[i] = tmp;
         if(!res.second)
         {
             break;
         }
     }
     std::stringstream ss;
     uint32_t kerSize{};
     ss << kernel;
     ss >> kerSize;
     ker.resize(kerSize);
     for(uint32_t i = 0; i < kerSize; ++i)
     {
         std::vector<KernelType> row(kerSize, 0.0f);
         for(uint32_t j = 0; j < kerSize; ++j)
         {
             KernelType tmp{};
             ss >> tmp;
             row[j] = tmp;
         }
         ker[i] = std::move(row);
     }
     return true;

  } while(false);
  ker =
      {
        {-1.0f,-1.0f,0.0f,1.0f,1.0f},
        {-1.0f,-1.0f,0.0f,1.0f,1.0f},
        { 0.0f, 0.0f,0.0f,0.0f,0.0f},
        {-1.0f,-1.0f,0.0f,1.0f,1.0f},
        {-1.0f,-1.0f,0.0f,1.0f,1.0f}
      };
  return true;
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
    ConvKernel ker;
    GetSettings(sock, ker);
    ConvHandler<unsigned char> convHandler
    (
        ker
    );
    auto processedBuff =
        ImageProcessor::Convolution(
            decomprBuffer,
            convHandler);
    auto compressedBuffer = JpegHelper::Compress(processedBuff);
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
