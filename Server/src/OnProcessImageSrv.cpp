#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "VideoDevice/VideoDevice.h"
#include <sstream>
#include "ImageProcessing/JpegHelper.h"
#include "ImageProcessing/ImageProcessor.h"

#include "OnProcessImageSrv.h"
#include "ServerCommandHandler.h"
#include <sys/socket.h>

void
OnProcessImageSrv::Handle(DescriptorHolder& sock)
{
  std::string fileName("Images/webcam_output_");
  auto& device = ServerCommandHandler::GetData().device;
  auto decomprBuffer = GetImageBufferFromDevice(device);
  ImageProcessor::ToGrayScale(decomprBuffer);
  auto compressedBuffer = JpegHelper::Compress(decomprBuffer);
  EResponse response = EResponse::SUCCESS;
  send(sock.Get(), &response, sizeof(response), 0);
  int x = 73;
  int y = 27;
  send(sock.Get(), &x, sizeof(x), 0);
  send(sock.Get(), &y, sizeof(y), 0);

  auto& imagesCount = ServerCommandHandler::GetData().imagesCount;
  std::ostringstream ss;
  ss << fileName << imagesCount++ << ".jpeg";
  JpegHelper::WriteBufferToFile(compressedBuffer, ss.str());

}
