#pragma once

#include <string>
#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "MappedBuffer.h"
#include "../Utils/DescriptorHolder.h"
#include "../Utils/CallWrapper.h"

class VideoDevice
{
public:
  explicit VideoDevice(const std::string& deviceName);
  MappedBuffer& GetRawBuffer();
private:
  template<typename T>
  std::pair<int, bool> AsyncIoctl(int request, T* pRequestData)
  {
    return mCallHandler.WaitForAsyncCall<decltype(CheckIoctl), CheckIoctl>
      (ioctl, mTimeout, mDescriptor.Get(), request, pRequestData);
  }
  void OpenDevice(const std::string& deviceName);
  void SetImageFormat();
  void InitBuffer();
private:
  DescriptorHolder mDescriptor{};
  MappedBuffer mBuffer{};
  CallWrapper mCallHandler;

  const std::chrono::milliseconds mTimeout{300};
};
