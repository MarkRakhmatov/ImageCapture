#pragma once
#include "VideoDevice/VideoDevice.h"

struct CommandHandlerData
{
  std::string deviceName{"/dev/video0"};
  VideoDevice device{deviceName};
  size_t imagesCount = 0;
};
