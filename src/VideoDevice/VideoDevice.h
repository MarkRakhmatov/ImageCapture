#pragma once

#include <string>
#include <linux/videodev2.h>

#include "MappedBuffer.h"
#include "../Utils/DescriptorHolder.h"
#include "../Utils/CallWrapper.h"

class VideoDevice
{
public:
  explicit VideoDevice(const std::string& deviceName);
  MappedBuffer& GetRawBuffer();
private:
  void OpenDevice(const std::string& deviceName);
  void SetImageFormat();
  void InitBuffer();
private:
  DescriptorHolder mDescriptor{};
  MappedBuffer mBuffer{};
  CallWrapper mCallHandler;

  const std::chrono::milliseconds mTimeout{250};
};
