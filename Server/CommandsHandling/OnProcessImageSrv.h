#pragma once
#include "Command.h"
#include "IOnCommandSrv.h"
#include "VideoDevice/VideoDevice.h"
#include <chrono>
#include <vector>

class OnProcessImageSrv: public IOnCommandSrv
{
public:
  ~OnProcessImageSrv() = default;
  EConnectionStatus Handle(Socket& sock);
private:
  EConnectionStatus SetupCamera(Socket& sock);
  EConnectionStatus ProcessImage(Socket& sock);
  EProcessImage ReadCommand(Socket& sock);
private:
  std::string mDeviceName{"/dev/video0"};
  VideoDevice mDevice{mDeviceName};
  size_t mImagesCount = 0;
  std::chrono::milliseconds mTimeout{300};
};
