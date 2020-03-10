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
#include "ServerCommandHandler.h"

EConnectionStatus
OnProcessImageSrv::Handle(Socket& sock)
{
  do
  {
    std::string fileName("Images/webcam_output_");
    auto& device = ServerCommandHandler::GetData().device;
    auto decomprBuffer = GetImageBufferFromDevice(device);
    if(!decomprBuffer.GetWidth() || !decomprBuffer.GetHeight())
    {
        std::cout<<"Failed to get buffer!"<<std::endl;
        break;
    }
    ImageProcessor::ToGrayScale(decomprBuffer);
    auto compressedBuffer = JpegHelper::Compress(decomprBuffer);
    EConnectionStatus response = EConnectionStatus::SUCCESS;
    sock.Send(&response, sizeof(response));
    int32_t x = 73;
    int32_t y = 27;
    auto res = sock.Send(&x, sizeof(x));
    if(!res.second)
    {
        break;
    }
    res = sock.Send(&y, sizeof(y));
    if(!res.second)
    {
        break;
    }

    auto& imagesCount = ServerCommandHandler::GetData().imagesCount;
    std::ostringstream ss;
    ss << fileName << imagesCount++ << ".jpeg";
    JpegHelper::WriteBufferToFile(compressedBuffer, ss.str());

    return EConnectionStatus::SUCCESS;
  }
  while(false);

  std::cout<<"Failed to send response!"<<std::endl;
  return EConnectionStatus::FAIL;
}
